/********************************************************************
*	�ļ�����:	config.h											*
*	����ʱ�䣺	2014/03/17											*
*	��   �� :	xzben												*
*	�ļ�����:	���ļ��Ǹ�����ֱ�ӵ��õ�ͷ�ļ����ڴ��ļ���ֻ��Ҫѡ���	*
*				�����ʵ�ƽ̨��config�ļ����в�������������				*
*********************************************************************/



#include "interal_types.h"
#include <cassert>

#ifndef __CONFIG_H__
#define __CONFIG_H__


/*************************************************
**	ѡ�� include ��Ӧƽ̨�� config_xxx.h �ļ�		**
**************************************************/

#include "config_win32.h"		/*	ѡ�� win32 ƽ̨	*/
//#include "config_linux.h"		/*	ѡ�� linux ƽ̨	*/	


/*************************************************
** ϵͳ��ʹ�õ������ռ�궨��,ʹ�������ռ���ҪΪ�� **
** ��ֹ���������������ͻ						    **
**************************************************/
#define BEGIN_NAMESPACE  namespace  XZBEN{
#define END_NAMESPACE	 }
#define USING_NAMESPACE  using namespace XZBEN;


/*************************************************
**	ϵͳ��ʹ�õ� ASSERT �궨��					**
**************************************************/
#define	ASSERT(__express)					assert((__express));
#define VERIFY(__express)					if( !(__express) ) { ASSERT(false); }



/*************************************************
**	ϵͳ��ʹ�õ� DELETE ��ȫ�궨��				**
**************************************************/
#define	SAFE_DELETE(__ptr)					if( nullptr != (__ptr) ) { delete __ptr; __ptr = nullptr; }
#define SAFE_DELETE_ARR(__ptr)				if( nullptr != (__ptr) ) { delete[] __ptr; __ptr = nullptr; }	

/*************************************************
**	ϵͳ��ʹ�õ� ����״̬��� 					**
**************************************************/
#define	SET_BIT(_dwCntrl, _bit)				((_dwCntrl)|(_bit))
#define GET_BIT(_dwCntrl, _bit)				((_dwCntrl)&(_bit))
#define DEL_BIT(_dwCntrl, _bit)				((_dwCntrl)&(~(_bit)))

#define SET_DEL_BIT(_dwCntrl, _bit, _bSet)	((_bSet) ? SET_BIT(_dwCntrl, _bit) : DEL_BIT(_dwCntrl, _bit))
#define QUERY_IS_SET_BIT(_dwCntrl, _bit)	GET_BIT(_dwCntrl, _bit)

#define SET_PTR_VALUD_SAFE(_ptr, _val)		if(nullptr != _ptr) { *_ptr = _val; }
/*************************************************
**	dll ���뵼�����ú� 							**
**************************************************/
# define EXPORT_DLL												__declspec (dllexport)
# define IMPORT_DLL												__declspec (dllimport)
# define EXPORT_C_DLL											extern "C" __declspec(dllexport)
# define IMPORT_C_DLL											extern "C" __declspec(dllimport)
# define EXPORT_SINGLETON_DECLARATION(T)						template class __declspec (dllexport) T
# define EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)	template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
# define IMPORT_SINGLETON_DECLARATION(T)						extern template class T
# define IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)	extern template class SINGLETON_TYPE <CLASS, LOCK>;

#ifdef  _OPENSOCKET_BUILD_
	#define EXP_IMP_DLL			EXPORT_DLL
	#define EXP_IMP_C_DLL		EXPORT_C_DLL
#else//_OPENSOCKET_BUILD_
	#define EXP_IMP_DLL			IMPORT_DLL
	#define	EXP_IMP_C_DLL		IMPORT_C_DLL
#endif//_OPENSOCKET_BUILD_

#endif // !__CONFIG_H__