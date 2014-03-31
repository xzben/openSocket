/********************************************************************
*	文件名称:	config.h											*
*	创建时间：	2014/03/17											*
*	作   者 :	xzben												*
*	文件功能:	本文件是给程序直接调用的头文件，在此文件中只需要选择包	*
*				含合适的平台的config文件就行不用做其它事情				*
*********************************************************************/



#include "interal_types.h"
#include <cassert>

#ifndef __CONFIG_H__
#define __CONFIG_H__


/*************************************************
**	选择 include 对应平台的 config_xxx.h 文件		**
**************************************************/

#include "config_win32.h"		/*	选择 win32 平台	*/
//#include "config_linux.h"		/*	选择 linux 平台	*/	


/*************************************************
** 系统中使用的命名空间宏定义,使用命名空间主要为了 **
** 防止和其它库的命名冲突						    **
**************************************************/
#define BEGIN_NAMESPACE  namespace  XZBEN{
#define END_NAMESPACE	 }
#define USING_NAMESPACE  using namespace XZBEN;


/*************************************************
**	系统中使用的 ASSERT 宏定义					**
**************************************************/
#define	ASSERT(__express)					assert((__express));
#define VERIFY(__express)					if( !(__express) ) { ASSERT(false); }



/*************************************************
**	系统中使用的 DELETE 安全宏定义				**
**************************************************/
#define	SAFE_DELETE(__ptr)					if( nullptr != (__ptr) ) { delete __ptr; __ptr = nullptr; }
#define SAFE_DELETE_ARR(__ptr)				if( nullptr != (__ptr) ) { delete[] __ptr; __ptr = nullptr; }	

/*************************************************
**	系统中使用的 设置状态标记 					**
**************************************************/
#define	SET_BIT(_dwCntrl, _bit)				((_dwCntrl)|(_bit))
#define GET_BIT(_dwCntrl, _bit)				((_dwCntrl)&(_bit))
#define DEL_BIT(_dwCntrl, _bit)				((_dwCntrl)&(~(_bit)))

#define SET_DEL_BIT(_dwCntrl, _bit, _bSet)	((_bSet) ? SET_BIT(_dwCntrl, _bit) : DEL_BIT(_dwCntrl, _bit))
#define QUERY_IS_SET_BIT(_dwCntrl, _bit)	GET_BIT(_dwCntrl, _bit)

#define SET_PTR_VALUD_SAFE(_ptr, _val)		if(nullptr != _ptr) { *_ptr = _val; }
/*************************************************
**	dll 导入导出配置宏 							**
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