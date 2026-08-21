/**************************************************************************
 * Copyright (c) 2013 TTTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Vienna, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file ApdbCfg.h
 *
 * \brief Definitions for Apdb.h
 *
 *      This file defines important settings for Apdb.h
 *
 **************************************************************************/
#ifndef _APDB_CFG_H
#define _APDB_CFG_H

#include "ptypes_xe167.h"

/* prototype for cstart */
extern void _cstart( void );

#ifndef APPL_START
#define APPL_START      ((ubyte4) &_cstart)
#endif
#define APDB_ADDRESS    0xC10000
#define APDB            volatile __huge const ApdbType
/* Use LOCATE_APDB at the definition of appl_db and nowhere else */
#define LOCATE_APDB     APDB __at(APDB_ADDRESS)

/* appl_db must be defined using LOCATE_APDB */
extern APDB appl_db;

#endif /* _APDB_CFG_H */
