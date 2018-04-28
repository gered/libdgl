#include "dglpcx.h"
#include "dglgfx.h"
#include "dglpal.h"
#include "dgldraw.h"
#include "dglerror.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    byte manufacturer;
    byte version;
    byte encoding;
    byte bpp;
    word x;
    word y;
    word width;
    word height;
    word horizontal_dpi;
    word vertical_dpi;
    byte ega_palette[48];
    byte reserved;
    byte num_color_planes;
    word bytes_per_line;
    word palette_type;
    word horizontal_size;
    word vertical_size;
    byte padding[54];
} PCX_HEADER;

SURFACE* pcx_load(const char *filename, byte *pcx_palette) {
    FILE *fp;
    PCX_HEADER header;
    int i, n, count, x, y;
    SURFACE *pcx;
    ubyte data;

    fp = fopen(filename, "rb");
    if (!fp) {
        dgl_set_error(DGL_IO_ERROR);
        return NULL;
    }

    n = fread(&header, sizeof(PCX_HEADER), 1, fp);
    if (n == -1) {
        dgl_set_error(DGL_IO_ERROR);
        fclose(fp);
        return NULL;
    }

    if (header.manufacturer != 10 ||
        header.version != 5 ||
        header.encoding != 1 ||
        header.bpp != 8) {
        dgl_set_error(DGL_PCX_BAD_FORMAT);
        fclose(fp);
        return NULL;
    }

    pcx = surface_create(header.width + 1, header.height + 1);

	i = 0;
    for (y = 0; y < (header.height + 1); ++y) {
		// write pixels out per-scanline (technically this is what the pcx
		// standard specifies, though a lot of pcx loaders don't do this).
	    x = 0;
		while (x < header.bytes_per_line) {
            // read pixel (or RLE count...)
            data = fgetc(fp);
			if ((data & 0xc0) == 0xc0) {
			   // was an RLE count, pixel is next byte
			   count = data & 0x3f;
			   data = fgetc(fp);
			} else {
			  count = 1;
			}

			// store this pixel colour the specified number of times
			while (count--) {
                if (x < pcx->width) {
				   pcx->pixels[i] = data;
				}
				++i;
				++x;
			}
		}
	}

    // read palette (only if needed)
    if (pcx_palette) {
        fseek(fp, -768, SEEK_END);

        n = fread(pcx_palette, 768, 1, fp);
        if (n == -1)
            goto pcx_load_error;

        for (i = 0; i < 768; ++i)
            pcx_palette[i] >>= 2;
    }

    fclose(fp);
    return pcx;

pcx_load_error:
    dgl_set_error(DGL_PCX_BAD_FORMAT);
    surface_free(pcx);
    fclose(fp);
    return NULL;
}

static boolean write_pcx_data(FILE *fp, int run_count, byte pixel) {
    int n;

    if ((run_count > 1) || ((pixel & 0xc0) == 0xc0)) {
        n = fputc(0xc0 | run_count, fp);
        if (n == -1)
            return FALSE;
    }
    n = fputc(pixel, fp);
    if (n == -1)
        return FALSE;

    return TRUE;
}

boolean pcx_save(const char *filename, const SURFACE *src, const byte *palette) {
    FILE *fp;
    int i, n, x, y;
    int run_count;
    byte pixel, run_pixel;
    byte r, g, b;
    boolean result;
    PCX_HEADER header;

    fp = fopen(filename, "wb");
    if (!fp) {
        dgl_set_error(DGL_IO_ERROR);
        return FALSE;
    }

    memset(&header, 0, sizeof(PCX_HEADER));
    header.manufacturer = 10;
    header.version = 5;
    header.encoding = 1;
    header.bpp = 8;
    header.x = 0;
    header.y = 0;
    header.width = src->width - 1;
    header.height = src->height - 1;
    header.horizontal_dpi = 0;
    header.vertical_dpi = 0;
    header.num_color_planes = 1;
    header.bytes_per_line = src->width;
    header.palette_type =  1;
    header.horizontal_size = 320;
    header.vertical_size = 200;

    n = fwrite(&header, sizeof(PCX_HEADER), 1, fp);
    if (n == -1)
        goto pcx_save_error;

    // write image data
    i = 0;
    for (y = 0; y < src->height; ++y) {
        // write one scanline at a time (breaking runs that could have
        // continue across scanlines in the process... as per pcx standard)
        run_count = 0;
        run_pixel = 0;
        for (x = 0; x < src->width; ++x) {
            pixel = src->pixels[i];
            ++i;

            if (run_count == 0) {
                run_count = 1;
                run_pixel = pixel;

            } else {
                if ((pixel != run_pixel) || (run_count >= 63)) {
                    result = write_pcx_data(fp, run_count, run_pixel);
                    if (!result)
                        goto pcx_save_error;

                    run_count = 1;
                    run_pixel = pixel;

                } else {
                    ++run_count;
                }
            }
        }

        // end the scanline, writing out whatever run we might have had going
        result = write_pcx_data(fp, run_count, run_pixel);
        if (!result)
            goto pcx_save_error;
    }

    fputc(12, fp);

    // use provided palette, otherwise use current vga palette
    if (palette) {
        for (i = 0; i < 768; ++i) {
            n = fputc(palette[i] << 2, fp);
            if (n == -1)
                goto pcx_save_error;
        }
    } else {
        for (i = 0; i < 256; ++i) {
            pal_get_color(i, &r, &g, &b);

            n = fputc(r << 2, fp);
            if (n == -1)
                goto pcx_save_error;
            n = fputc(g << 2, fp);
            if (n == -1)
                goto pcx_save_error;
            n = fputc(b << 2, fp);
            if (n == -1)
                goto pcx_save_error;
        }
    }

    fclose(fp);
    return TRUE;

pcx_save_error:
    dgl_set_error(DGL_IO_ERROR);
    fclose(fp);
    return FALSE;
}

