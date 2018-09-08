#ifndef Contant_h__
#define Contant_h__
/*!
 * \file Contant.h
 *
 * \author Administrator
 * \date 9월 2015
 *
 * 상수값을 보관한다.
 */

const _ushort WINSIZEX = 1920;
const _ushort WINSIZEY = 1080;
extern _ulong VTXCNTX/* = 129*/;
extern _ulong VTXCNTZ/* = 129*/;
extern _ulong VTXITV/* = 1*/;

const _vec3 g_vLook = _vec3(1.f, 0.f, 0.f);


enum RESOURCETYPE {RESOURCE_STATIC, RESOURCE_LOGO, RESOURCE_STAGE, RESOURCE_END};


#endif // Contant_h__
