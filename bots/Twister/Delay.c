void delay( int cnt )
{
    asm( "1: dbra %0,1b" : : "d" (cnt) );
}

void delay2( int msec )
{
	asm( "1: move.l #0x12345678,%d1" );
	asm( "2: dbra %d1,2b" );
	asm( "   dbra %0,1b" : : "d" (msec) );
}

void delay3( int msec )
{
    do
    {
        int x = 0x12345678;
        do
        {
            ;
        } while ( --x > 0 );
    }
    while ( --msec != 0 );
}
