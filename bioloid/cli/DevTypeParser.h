/****************************************************************************
*
*   Copyright (c) 2009 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*   See README and COPYING for more details.
*
****************************************************************************/
/**
*
*   @file   DevTypeParser.h
*
*   @brief  Implements a parser for parsing device type files.
*
****************************************************************************/

#if !defined( DEVTYPEPARSER_H )
#define DEVTYPEPARSER_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include <stdio.h>

#include "Log.h"
#include "bioloid-reg.h"
#include "StrToken.h"

// ---- Class Declarations --------------------------------------------------

class DevTypeParser
{
public:
    DevTypeParser();

    ~DevTypeParser();

    typedef void (*AddDevTypeFunc)( BLD_DevType_t *devType );

    bool    ParseFile( const char *fileName, AddDevTypeFunc addDevTypeFunc );

private:

    bool    ParseLine();
    bool    ParseRegister( BLD_Reg_t *reg );

    FILE           *m_fs;
    BLD_DevType_t  *m_devType;
    const char     *m_fileName;
    char            m_lineBuf[ 200 ];
    char            m_token[ 20 ];
    StrTokenizer    m_line;
    unsigned        m_lineNum;

    unsigned        m_allocRegs;

    AddDevTypeFunc  m_addDevTypeFunc;
};


#endif  // DEVTYPEPARSER_H

