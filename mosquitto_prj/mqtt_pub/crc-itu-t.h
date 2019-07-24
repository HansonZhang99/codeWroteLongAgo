/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  crc-itu-t.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(03/30/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/30/2019 02:46:27 PM"
 *                 
 ********************************************************************************/

#ifndef CRC_ITU_T_H
#define CRC_ITU_T_H

#define MAGIC_CRC           0x1E50

const unsigned short  crc_itu_t_table[256];
unsigned short crc_itu_t(unsigned short crc, const unsigned char *buffer, unsigned int len);


#endif /*  CRC_ITU_T_H */

