
// Use in place of Ym2413_Emu.cpp and ym2413.c to disable support for this chip

// Game_Music_Emu https://bitbucket.org/mpyne/game-music-emu/

#include "Ym2413_Emu.h"

#include <assert.h>
// include the C file directly - the correct way to do this would be to link the C file
// in but I don't know cmake that well :(
#include "emu2413/emu2413.c"

Ym2413_Emu::~Ym2413_Emu()
{
	if ( opll )
	{
		OPLL_delete( opll );
	}
}

Ym2413_Emu::Ym2413_Emu()
{
	opll = 0;
}

int Ym2413_Emu::set_rate( double sample_rate, double clock_rate )
{
	if ( opll )
	{
		OPLL_delete( opll );
		opll = 0;
	}
	
	opll = OPLL_new( clock_rate, sample_rate );
	if ( !opll )
		return 1;
	
	reset();
	return 0;
}

void Ym2413_Emu::reset()
{
	OPLL_reset( opll );
	OPLL_reset_patch( opll, 0 );
	OPLL_setMask( opll, 0 );
	OPLL_set_quality( opll, 0 );
}

void Ym2413_Emu::write( int addr, int data )
{
	OPLL_writeReg( opll, addr, data );
}

void Ym2413_Emu::mute_voices( int mask )
{
	OPLL_setMask( opll, mask );
}

void Ym2413_Emu::run( int pair_count, sample_t* out )
{
	while ( pair_count-- )
	{
		int s = OPLL_calc( opll );
		out [0] = s;
		out [1] = s;
		out += 2;
	}
}

