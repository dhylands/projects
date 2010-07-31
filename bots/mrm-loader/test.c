#define QSM_BASE (0xfffffc00L)

#define QSM_MCR		(*(short int *) (QSM_BASE))
#define QSM_TEST	(*(short int *) (QSM_BASE+0x02))
#define QSM_QILR_QIVR	(*(short int *) (QSM_BASE+0X04))
#define QSM_SCCR0       (*(short int *) (QSM_BASE+0x08))
#define QSM_SCCR1	(*(short int *) (QSM_BASE+0x0a))
#define QSM_SCSR	(*(short int *) (QSM_BASE+0x0c))
#define QSM_SCDR	(*(short int *) (QSM_BASE+0x0e))
#define QSM_PORTQS	(*(short int *) (QSM_BASE+0x15))
#define QSM_PQSPAR_DDR	(*(short int *) (QSM_BASE+0x16))
#define QSM_SRCR0	(*(short int *) (QSM_BASE+0x18))
#define QSM_SRCR1	(*(short int *) (QSM_BASE+0x1a))
#define QSM_SPCR2	(*(short int *) (QSM_BASE+0x1c))
#define QSM_SPCR3_SPSR	(*(short int *) (QSM_BASE+0x1e))

#define QSM_RXRAM 	(QSM_BASE+0x100)
#define QSM_TXRAM	(QSM_BASE+0x120)
#define QSM_CMDRAM	(QSM_BASE+0x140)

int sendingbyte(void) {
 
  return (QSM_SCSR & 0x80) == 0;
}

void wait_xmitr( void )
{
    while ( sendingbyte() )
    {
        
    }
}

void putc( int ch )
{
    wait_xmitr();
    QSM_SCDR = (char)ch;
    wait_xmitr();
}

void puts( const char *s )
{
    while ( *s != '\0' )
    {
        putc( *s );
        s++;
    }
}

void foo( void )
{
    puts( "Testing\n" );
}
