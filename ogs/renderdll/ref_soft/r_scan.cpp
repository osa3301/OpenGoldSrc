/*
 *	This file is part of OGS Engine
 *	Copyright (C) 2016-2017 OGS Dev Team
 *
 *	OGS Engine is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	OGS Engine is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OGS Engine.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	In addition, as a special exception, the author gives permission to
 *	link the code of OGS Engine with the Half-Life Game Engine ("GoldSrc/GS
 *	Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *	L.L.C ("Valve").  You must obey the GNU General Public License in all
 *	respects for all of the code used other than the GoldSrc Engine and MODs
 *	from Valve.  If you modify this file, you may extend this exception
 *	to your version of the file, but you are not obligated to do so.  If
 *	you do not wish to do so, delete this exception statement from your
 *	version.
 */

/// @file
/// @brief Portable C scan-level rasterization code, all pixel depths

#include "r_local.hpp"

unsigned char *r_turb_pbase, *r_turb_pdest;
fixed16_t      r_turb_s, r_turb_t, r_turb_sstep, r_turb_tstep;
int *          r_turb_turb;
int            r_turb_spancount;

void D_DrawTurbulent8Span();

/*
=============
D_WarpScreen

this performs a slight compression of the screen at the same time as
the sine warp, to keep the edges from wrapping
=============
*/
void D_WarpScreen()
{
	int    w, h;
	int    u, v, u2, v2;
	byte * dest;
	int *  turb;
	int *  col;
	byte **row;

	static int   cached_width, cached_height;
	static byte *rowptr[1200 + AMP2 * 2];
	static int   column[1600 + AMP2 * 2];

	//
	// these are constant over resolutions, and can be saved
	//
	w = r_newrefdef.width;
	h = r_newrefdef.height;
	if(w != cached_width || h != cached_height)
	{
		cached_width  = w;
		cached_height = h;
		for(v = 0; v < h + AMP2 * 2; v++)
		{
			v2        = (int)((float)v / (h + AMP2 * 2) * r_refdef.vrect.height);
			rowptr[v] = r_warpbuffer + (WARP_WIDTH * v2);
		}

		for(u = 0; u < w + AMP2 * 2; u++)
		{
			u2        = (int)((float)u / (w + AMP2 * 2) * r_refdef.vrect.width);
			column[u] = u2;
		}
	}

	turb = intsintable + ((int)(r_newrefdef.time * SPEED) & (CYCLE - 1));
	dest = vid.buffer + r_newrefdef.y * vid.rowbytes + r_newrefdef.x;

	for(v = 0; v < h; v++, dest += vid.rowbytes)
	{
		col = &column[turb[v]];
		row = &rowptr[v];
		for(u = 0; u < w; u += 4)
		{
			dest[u + 0] = row[turb[u + 0]][col[u + 0]];
			dest[u + 1] = row[turb[u + 1]][col[u + 1]];
			dest[u + 2] = row[turb[u + 2]][col[u + 2]];
			dest[u + 3] = row[turb[u + 3]][col[u + 3]];
		}
	}
}

#if !id386

/*
=============
D_DrawTurbulent8Span
=============
*/
void D_DrawTurbulent8Span()
{
	int sturb, tturb;

	do
	{
		sturb           = ((r_turb_s + r_turb_turb[(r_turb_t >> 16) & (CYCLE - 1)]) >> 16) & 63;
		tturb           = ((r_turb_t + r_turb_turb[(r_turb_s >> 16) & (CYCLE - 1)]) >> 16) & 63;
		*r_turb_pdest++ = *(r_turb_pbase + (tturb << 6) + sturb);
		r_turb_s += r_turb_sstep;
		r_turb_t += r_turb_tstep;
	}
	while(--r_turb_spancount > 0);
}

#endif // !id386

/*
=============
Turbulent8
=============
*/
void Turbulent8(espan_t *pspan)
{
	int       count;
	fixed16_t snext, tnext;
	float     sdivz, tdivz, zi, z, du, dv, spancountminus1;
	float     sdivz16stepu, tdivz16stepu, zi16stepu;

	r_turb_turb = sintable + ((int)(r_newrefdef.time * SPEED) & (CYCLE - 1));

	r_turb_sstep = 0; // keep compiler happy
	r_turb_tstep = 0; // ditto

	r_turb_pbase = (unsigned char *)cacheblock;

	sdivz16stepu = d_sdivzstepu * 16;
	tdivz16stepu = d_tdivzstepu * 16;
	zi16stepu    = d_zistepu * 16;

	do
	{
		r_turb_pdest = (unsigned char *)((byte *)d_viewbuffer +
		                                 (r_screenwidth * pspan->v) + pspan->u);

		count = pspan->count;

		// calculate the initial s/z, t/z, 1/z, s, and t and clamp
		du = (float)pspan->u;
		dv = (float)pspan->v;

		sdivz = d_sdivzorigin + dv * d_sdivzstepv + du * d_sdivzstepu;
		tdivz = d_tdivzorigin + dv * d_tdivzstepv + du * d_tdivzstepu;
		zi    = d_ziorigin + dv * d_zistepv + du * d_zistepu;
		z     = (float)0x10000 / zi; // prescale to 16.16 fixed-point

		r_turb_s = (int)(sdivz * z) + sadjust;
		if(r_turb_s > bbextents)
			r_turb_s = bbextents;
		else if(r_turb_s < 0)
			r_turb_s = 0;

		r_turb_t = (int)(tdivz * z) + tadjust;
		if(r_turb_t > bbextentt)
			r_turb_t = bbextentt;
		else if(r_turb_t < 0)
			r_turb_t = 0;

		do
		{
			// calculate s and t at the far end of the span
			if(count >= 16)
				r_turb_spancount = 16;
			else
				r_turb_spancount = count;

			count -= r_turb_spancount;

			if(count)
			{
				// calculate s/z, t/z, zi->fixed s and t at far end of span,
				// calculate s and t steps across span by shifting
				sdivz += sdivz16stepu;
				tdivz += tdivz16stepu;
				zi += zi16stepu;
				z = (float)0x10000 / zi; // prescale to 16.16 fixed-point

				snext = (int)(sdivz * z) + sadjust;
				if(snext > bbextents)
					snext = bbextents;
				else if(snext < 16)
					snext = 16; // prevent round-off error on <0 steps from
				//  from causing overstepping & running off the
				//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if(tnext > bbextentt)
					tnext = bbextentt;
				else if(tnext < 16)
					tnext = 16; // guard against round-off error on <0 steps

				r_turb_sstep = (snext - r_turb_s) >> 4;
				r_turb_tstep = (tnext - r_turb_t) >> 4;
			}
			else
			{
				// calculate s/z, t/z, zi->fixed s and t at last pixel in span (so
				// can't step off polygon), clamp, calculate s and t steps across
				// span by division, biasing steps low so we don't run off the
				// texture
				spancountminus1 = (float)(r_turb_spancount - 1);
				sdivz += d_sdivzstepu * spancountminus1;
				tdivz += d_tdivzstepu * spancountminus1;
				zi += d_zistepu * spancountminus1;
				z     = (float)0x10000 / zi; // prescale to 16.16 fixed-point
				snext = (int)(sdivz * z) + sadjust;
				if(snext > bbextents)
					snext = bbextents;
				else if(snext < 16)
					snext = 16; // prevent round-off error on <0 steps from
				//  from causing overstepping & running off the
				//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if(tnext > bbextentt)
					tnext = bbextentt;
				else if(tnext < 16)
					tnext = 16; // guard against round-off error on <0 steps

				if(r_turb_spancount > 1)
				{
					r_turb_sstep = (snext - r_turb_s) / (r_turb_spancount - 1);
					r_turb_tstep = (tnext - r_turb_t) / (r_turb_spancount - 1);
				}
			}

			r_turb_s = r_turb_s & ((CYCLE << 16) - 1);
			r_turb_t = r_turb_t & ((CYCLE << 16) - 1);

			D_DrawTurbulent8Span();

			r_turb_s = snext;
			r_turb_t = tnext;

		} while(count > 0);

	} while((pspan = pspan->pnext) != NULL);
}

//====================
//PGM
/*
=============
NonTurbulent8 - this is for drawing scrolling textures. they're warping water textures
	but the turbulence is automatically 0.
=============
*/
void NonTurbulent8(espan_t *pspan)
{
	int       count;
	fixed16_t snext, tnext;
	float     sdivz, tdivz, zi, z, du, dv, spancountminus1;
	float     sdivz16stepu, tdivz16stepu, zi16stepu;

	//	r_turb_turb = sintable + ((int)(r_newrefdef.time*SPEED)&(CYCLE-1));
	r_turb_turb = blanktable;

	r_turb_sstep = 0; // keep compiler happy
	r_turb_tstep = 0; // ditto

	r_turb_pbase = (unsigned char *)cacheblock;

	sdivz16stepu = d_sdivzstepu * 16;
	tdivz16stepu = d_tdivzstepu * 16;
	zi16stepu    = d_zistepu * 16;

	do
	{
		r_turb_pdest = (unsigned char *)((byte *)d_viewbuffer +
		                                 (r_screenwidth * pspan->v) + pspan->u);

		count = pspan->count;

		// calculate the initial s/z, t/z, 1/z, s, and t and clamp
		du = (float)pspan->u;
		dv = (float)pspan->v;

		sdivz = d_sdivzorigin + dv * d_sdivzstepv + du * d_sdivzstepu;
		tdivz = d_tdivzorigin + dv * d_tdivzstepv + du * d_tdivzstepu;
		zi    = d_ziorigin + dv * d_zistepv + du * d_zistepu;
		z     = (float)0x10000 / zi; // prescale to 16.16 fixed-point

		r_turb_s = (int)(sdivz * z) + sadjust;
		if(r_turb_s > bbextents)
			r_turb_s = bbextents;
		else if(r_turb_s < 0)
			r_turb_s = 0;

		r_turb_t = (int)(tdivz * z) + tadjust;
		if(r_turb_t > bbextentt)
			r_turb_t = bbextentt;
		else if(r_turb_t < 0)
			r_turb_t = 0;

		do
		{
			// calculate s and t at the far end of the span
			if(count >= 16)
				r_turb_spancount = 16;
			else
				r_turb_spancount = count;

			count -= r_turb_spancount;

			if(count)
			{
				// calculate s/z, t/z, zi->fixed s and t at far end of span,
				// calculate s and t steps across span by shifting
				sdivz += sdivz16stepu;
				tdivz += tdivz16stepu;
				zi += zi16stepu;
				z = (float)0x10000 / zi; // prescale to 16.16 fixed-point

				snext = (int)(sdivz * z) + sadjust;
				if(snext > bbextents)
					snext = bbextents;
				else if(snext < 16)
					snext = 16; // prevent round-off error on <0 steps from
				//  from causing overstepping & running off the
				//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if(tnext > bbextentt)
					tnext = bbextentt;
				else if(tnext < 16)
					tnext = 16; // guard against round-off error on <0 steps

				r_turb_sstep = (snext - r_turb_s) >> 4;
				r_turb_tstep = (tnext - r_turb_t) >> 4;
			}
			else
			{
				// calculate s/z, t/z, zi->fixed s and t at last pixel in span (so
				// can't step off polygon), clamp, calculate s and t steps across
				// span by division, biasing steps low so we don't run off the
				// texture
				spancountminus1 = (float)(r_turb_spancount - 1);
				sdivz += d_sdivzstepu * spancountminus1;
				tdivz += d_tdivzstepu * spancountminus1;
				zi += d_zistepu * spancountminus1;
				z     = (float)0x10000 / zi; // prescale to 16.16 fixed-point
				snext = (int)(sdivz * z) + sadjust;
				if(snext > bbextents)
					snext = bbextents;
				else if(snext < 16)
					snext = 16; // prevent round-off error on <0 steps from
				//  from causing overstepping & running off the
				//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if(tnext > bbextentt)
					tnext = bbextentt;
				else if(tnext < 16)
					tnext = 16; // guard against round-off error on <0 steps

				if(r_turb_spancount > 1)
				{
					r_turb_sstep = (snext - r_turb_s) / (r_turb_spancount - 1);
					r_turb_tstep = (tnext - r_turb_t) / (r_turb_spancount - 1);
				}
			}

			r_turb_s = r_turb_s & ((CYCLE << 16) - 1);
			r_turb_t = r_turb_t & ((CYCLE << 16) - 1);

			D_DrawTurbulent8Span();

			r_turb_s = snext;
			r_turb_t = tnext;

		} while(count > 0);

	} while((pspan = pspan->pnext) != NULL);
}
//PGM
//====================

#if !id386

/*
=============
D_DrawSpans16

  FIXME: actually make this subdivide by 16 instead of 8!!!
=============
*/
void D_DrawSpans16(espan_t *pspan)
{
	int            count, spancount;
	unsigned char *pbase, *pdest;
	fixed16_t      s, t, snext, tnext, sstep, tstep;
	float          sdivz, tdivz, zi, z, du, dv, spancountminus1;
	float          sdivz8stepu, tdivz8stepu, zi8stepu;

	sstep = 0; // keep compiler happy
	tstep = 0; // ditto

	pbase = (unsigned char *)cacheblock;

	sdivz8stepu = d_sdivzstepu * 8;
	tdivz8stepu = d_tdivzstepu * 8;
	zi8stepu    = d_zistepu * 8;

	do
	{
		pdest = (unsigned char *)((byte *)d_viewbuffer +
		                          (r_screenwidth * pspan->v) + pspan->u);

		count = pspan->count;

		// calculate the initial s/z, t/z, 1/z, s, and t and clamp
		du = (float)pspan->u;
		dv = (float)pspan->v;

		sdivz = d_sdivzorigin + dv * d_sdivzstepv + du * d_sdivzstepu;
		tdivz = d_tdivzorigin + dv * d_tdivzstepv + du * d_tdivzstepu;
		zi    = d_ziorigin + dv * d_zistepv + du * d_zistepu;
		z     = (float)0x10000 / zi; // prescale to 16.16 fixed-point

		s = (int)(sdivz * z) + sadjust;
		if(s > bbextents)
			s = bbextents;
		else if(s < 0)
			s = 0;

		t = (int)(tdivz * z) + tadjust;
		if(t > bbextentt)
			t = bbextentt;
		else if(t < 0)
			t = 0;

		do
		{
			// calculate s and t at the far end of the span
			if(count >= 8)
				spancount = 8;
			else
				spancount = count;

			count -= spancount;

			if(count)
			{
				// calculate s/z, t/z, zi->fixed s and t at far end of span,
				// calculate s and t steps across span by shifting
				sdivz += sdivz8stepu;
				tdivz += tdivz8stepu;
				zi += zi8stepu;
				z = (float)0x10000 / zi; // prescale to 16.16 fixed-point

				snext = (int)(sdivz * z) + sadjust;
				if(snext > bbextents)
					snext = bbextents;
				else if(snext < 8)
					snext = 8; // prevent round-off error on <0 steps from
				//  from causing overstepping & running off the
				//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if(tnext > bbextentt)
					tnext = bbextentt;
				else if(tnext < 8)
					tnext = 8; // guard against round-off error on <0 steps

				sstep = (snext - s) >> 3;
				tstep = (tnext - t) >> 3;
			}
			else
			{
				// calculate s/z, t/z, zi->fixed s and t at last pixel in span (so
				// can't step off polygon), clamp, calculate s and t steps across
				// span by division, biasing steps low so we don't run off the
				// texture
				spancountminus1 = (float)(spancount - 1);
				sdivz += d_sdivzstepu * spancountminus1;
				tdivz += d_tdivzstepu * spancountminus1;
				zi += d_zistepu * spancountminus1;
				z     = (float)0x10000 / zi; // prescale to 16.16 fixed-point
				snext = (int)(sdivz * z) + sadjust;
				if(snext > bbextents)
					snext = bbextents;
				else if(snext < 8)
					snext = 8; // prevent round-off error on <0 steps from
				//  from causing overstepping & running off the
				//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if(tnext > bbextentt)
					tnext = bbextentt;
				else if(tnext < 8)
					tnext = 8; // guard against round-off error on <0 steps

				if(spancount > 1)
				{
					sstep = (snext - s) / (spancount - 1);
					tstep = (tnext - t) / (spancount - 1);
				}
			}

			do
			{
				*pdest++ = *(pbase + (s >> 16) + (t >> 16) * cachewidth);
				s += sstep;
				t += tstep;
			} while(--spancount > 0);

			s = snext;
			t = tnext;

		} while(count > 0);

	} while((pspan = pspan->pnext) != NULL);
}

#endif

#if !id386

/*
=============
D_DrawZSpans
=============
*/
void D_DrawZSpans(espan_t *pspan)
{
	int      count, doublecount, izistep;
	int      izi;
	short *  pdest;
	unsigned ltemp;
	float    zi;
	float    du, dv;

	// FIXME: check for clamping/range problems
	// we count on FP exceptions being turned off to avoid range problems
	izistep = (int)(d_zistepu * 0x8000 * 0x10000);

	do
	{
		pdest = d_pzbuffer + (d_zwidth * pspan->v) + pspan->u;

		count = pspan->count;

		// calculate the initial 1/z
		du = (float)pspan->u;
		dv = (float)pspan->v;

		zi = d_ziorigin + dv * d_zistepv + du * d_zistepu;
		// we count on FP exceptions being turned off to avoid range problems
		izi = (int)(zi * 0x8000 * 0x10000);

		if((long)pdest & 0x02)
		{
			*pdest++ = (short)(izi >> 16);
			izi += izistep;
			count--;
		}

		if((doublecount = count >> 1) > 0)
		{
			do
			{
				ltemp = izi >> 16;
				izi += izistep;
				ltemp |= izi & 0xFFFF0000;
				izi += izistep;
				*(int *)pdest = ltemp;
				pdest += 2;
			} while(--doublecount > 0);
		}

		if(count & 1)
			*pdest = (short)(izi >> 16);

	} while((pspan = pspan->pnext) != NULL);
}

#endif
