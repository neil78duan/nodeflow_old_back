/* file logicDataType.h
 *
 * create by duan 
 * 2015-5-8
 *
 */

#ifndef _LOGIC_DATA_TYPE_H_ 
#define _LOGIC_DATA_TYPE_H_

#include "nd_common/nd_define.h"
#include "ndapplib/nd_iBaseObj.h"
#include "logic_parser/logicApi4c.h"
#include <string>
#include <vector>
#include <map>

#define _ARRAR_BEGIN_MARK '['
#define _ARRAR_END_MARK ']'
#define _ARRAY_SPLITE_MARK ','

class LogicUserDefStruct;

typedef int(*logic_print)(void *pf, const char *stm, ...);

enum DBL_ELEMENT_TYPE
{
	OT_INT = 0x0,
	OT_FLOAT = 0x1,
	OT_STRING = 0x2,
	OT_INT8 = 0x3,
	OT_INT16 = 0x4,
	OT_INT64 = 0x5,
	OT_BOOL = 0x6,

	OT_ARRAY = 0x7, // 数组     {array_value_type=TEXT|a;b;c}
	//OT_ARRAY_2D = 0x8, // 数组     {array_value_type=TEXT|a;b;c}
	OT_TIME = 0x8,
	OT_VARIABLE = 0x9,  //variant in game-logic-vm
	OT_PARAM = 0xa,	// argment/param in call game-logic-script
	OT_LAST_RET = 0xb,	//last step return value 
	OT_USER_DEFINED = 0xc,		//json data

	OT_COMPILE_TIME = 0xd, //compile time 
	OT_FUNCTION_NAME = 0xe, //function name 
	OT_SCRIPT_MODULE_NAME = 0xf, //function name
	OT_BINARY_DATA = 0x10,		//binary-data
	OT_USER_DEFINED_ARRAY = 0X11,
	OT_AUTO = 0x12,		//auto type export by compile, if not assert string
	OT_ORG_STREAM = 0x13,	//org net message stream data, include format type-byte
	OT_CLOSURE_NAME= 0x14 ,	// closure name 

	OT_OBJECT_VOID = 0x20,		//game object , c_address of object
	OT_OBJ_MSGSTREAM = 0x21,		//input message stream
	OT_OBJ_BASE_OBJ = 0x22,			//class of  NDIBaseObj
	OT_OBJ_NDHANDLE = 0x23,
	OT_OBJ_NDOBJECT_unused = 0x24,
	OT_OBJ_OUT_MSGSTREAM_unused = 0x25,	//output message
	OT_ATTR_DATA = 0x26, //role attribute data
	OT_FILE_STREAM = 0X27, // file binary stream
};

#define OT_OBJ_NDOBJECT OT_OBJ_BASE_OBJ 

enum eBitOperate
{
	E_BIT_AND,
	E_BIT_OR,
	E_BIT_XOR,
	E_BIT_NOT,
	E_BIT_LEFT_MOVE,
	E_BIT_RIGHT_MOVE
};
#pragma pack(push, 4)
//整形数组/bool
struct dbl_intarray
{
	size_t number;
	size_t capacity;
	int data[1];
};
struct dbl_binary
{
	size_t size;
	size_t capacity;
	char data[1];
};
//浮点数组
struct dbl_floatarray
{
	size_t number;
	size_t capacity;
	float data[1];
};

//浮点数组
struct dbl_int64array
{
	size_t number;
	size_t capacity;
	NDUINT64 data[1];
};

//文本数组
struct dbl_strarray
{
	size_t number;
	size_t capacity;
	char* data[1];
};
struct  dbl_userDefArray
{
	size_t number;
	size_t capacity;
	LogicUserDefStruct *data[1];
};


struct _attrDataBin {
	int count;
	struct {
		NDUINT8 aid;
		float val;
	}datas[256];
};
struct dbl_attr {
	size_t number;
	_attrDataBin attr;
};

struct dbl_element_base
{
	bool isInit;
	union {
		int i_val;
		float f_val;
		NDUINT64 i64_val;
		char *str_val;
		LogicUserDefStruct *_userDef;
		void *_data;
		dbl_intarray *_i_arr;
		dbl_floatarray *_f_arr;
		dbl_strarray *_str_arr;
		dbl_int64array *_int64_arr;
		dbl_binary *_bin;
		dbl_userDefArray *_arr_user;
		dbl_attr *_attr_data;
	};
	
};

#pragma pack(pop)


//数据单元
class LOGIC_PARSER_CLASS LogicDataObj
{
public:
	LogicDataObj(dbl_element_base *data, DBL_ELEMENT_TYPE etype, DBL_ELEMENT_TYPE sub_etype);
	LogicDataObj(const LogicDataObj &r);
	LogicDataObj();
	~LogicDataObj();
	//init
	//init set vale
	LogicDataObj(int a, int type=OT_INT) {init() ;InitSet(a,type);}
	LogicDataObj(bool a) {init() ;InitSet(a);}
	LogicDataObj(float a) {init() ;InitSet(a);}
	LogicDataObj(char *str1) {init() ;InitSet(str1);}
	LogicDataObj(const char *str1)  {init() ;InitSet(str1);}
	LogicDataObj(int *arr, int size) {init() ;InitSet(arr, size);}
	LogicDataObj(float *arr, int size) {init() ;InitSet(arr, size);}
	LogicDataObj(NDUINT64 *arr, int size) { init(); InitSet(arr, size); }
	LogicDataObj(const char *arr[], int size) { init(); InitSet(arr, size); }

	LogicDataObj(void *object, DBL_ELEMENT_TYPE type = OT_OBJECT_VOID)  {init() ;InitSet(object, type);}
	LogicDataObj(NDIBaseObj *pObj) { init(); InitSet((void*)pObj, OT_OBJ_BASE_OBJ); }
	LogicDataObj(nd_handle handle) { init(); InitSet((void*)handle, OT_OBJ_NDHANDLE); }
	LogicDataObj(void *binary, size_t size, DBL_ELEMENT_TYPE eleType = OT_BINARY_DATA) {init() ;InitSet(binary,size,eleType);}
	LogicDataObj(NDUINT64 a) {init() ;InitSet(a);}
	LogicDataObj(NDUINT16 a) {init() ;InitSet(a);}
	LogicDataObj(NDUINT8 a) {init() ;InitSet(a);}
	LogicDataObj(time_t t) {init() ;InitSet(t);}
	LogicDataObj(const LogicUserDefStruct &u) {init() ;InitSet(u);}
	
	static DBL_ELEMENT_TYPE getTypeFromName(const char *typeName);
	static DBL_ELEMENT_TYPE getTypeFromValue(const char *valText);
	static int getCellType(const char *celltext, int &subType, bool forceFloatToInt=true);
	
	LogicDataObj &operator =(const LogicDataObj &r);
	LogicDataObj &operator =(const char *text);
	LogicDataObj &operator =(int val);
	LogicDataObj &operator =(float val);
	LogicDataObj &operator =(const std::string &val);
	LogicDataObj &operator =(const LogicUserDefStruct &u);

	bool operator == (const LogicDataObj &r) const;
	bool operator != (const LogicDataObj &r) const;
	bool operator < (const LogicDataObj &r) const ;

	//init set vale
	void InitSet(int a, int type= OT_INT);
	void InitSet(bool a);
	void InitSet(float a);
	void InitSet(char *str1);
	void InitSet(const char *str1);
	void InitSet(int *arr, int size);
	void InitSet(float *arr, int size);
	void InitSet(const char *arr[], int size);
	void InitSet(NDUINT64 *arr, int size);
	void InitSet(const LogicUserDefStruct *arr[], int size);
	void InitSet(void *object, DBL_ELEMENT_TYPE type = OT_OBJECT_VOID);
	void InitSet(void *binary, size_t size, DBL_ELEMENT_TYPE eleType = OT_BINARY_DATA);
	void InitSet(NDUINT64 a);
	void InitSet(NDUINT16 a);
	void InitSet(NDUINT8 a);
	void InitSet(time_t t);
	void InitSet(const LogicUserDefStruct &u);
	void InitSet(NDIBaseObj*obj) {	return InitSet((void*)obj, OT_OBJ_NDOBJECT); }
	bool InitReservedArray(size_t size, int attay_type = OT_INT);
	bool SetArray(const LogicDataObj &data, int index);	
	bool pushArray(const LogicDataObj &data);
	void InitFromTxt(const char *valText);
	bool InitTypeFromTxt(const char *valText, DBL_ELEMENT_TYPE datType=OT_STRING);

	bool GetVal(NDUINT64 &a) const ;
	bool GetVal(NDUINT16 &a)const;
	bool GetVal(NDUINT8 &a)const;
	bool GetVal(time_t &t)const;
	bool GetVal(float &a)const;
	bool GetVal(int &a)const;
	bool GetVal(bool &a)const;
	bool GetVal(char *buf, size_t size)const;
	bool GetVal(int *arr, size_t &size)const;
	bool GetVal(float *arr, size_t &size)const;

	int ReadStream(const char *streamBuf, size_t data_len, int streamByteOrder);
	int WriteStream(char *streamBuf, size_t buf_size, int streamByteOrder )const;
	int GetInt() const;
	int GetRoundInt() const;
	NDUINT64 GetInt64() const;
	bool GetBool() const;
	float GetFloat() const;
	const char *GetText() const;		//only used when type == OT_STRING
	std::string GetString() const;
	void *GetObjectAddr() const;
	void *GetBinary() const;
	size_t GetBinarySize() const;
	NDIBaseObj *GetNDObj() const;
	nd_handle GetNDHandle()const;

	LogicDataObj GetArray(int index)const;
	int GetarrayInt(int index) const;
	bool GetarrayBool(int index) const;
	float GetarrayFloat(int index) const;
	NDUINT64 GetarrayInt64(int index) const;
	const char *GetarrayText(int index) const;
	std::string GetarrayString(int index) const;
	const LogicUserDefStruct *GetarrayUser(int index) const;

	bool toArray(LogicDataObj &outVal)const;		//convert this(string) to array
	bool StringToArrayInt(const char *text); 
	bool StringToArrayFloat(const char *text);
	bool StringToArrayString(const char *text);
	static bool TestStringIsArray(const char *text);
		
	time_t GetTime() const;
	int GetTimeYear()const;
	int GetTimeMonth()const;
	int GetTimeDay()const;
	int GetTimeHour()const;
	int GetTimeMinute()const;
	int GetTimeSecond()const;
	int GetTimeWeekDay()const;
	
	LogicDataObj getUserDefMember(const char *name) const;
	void setUserDefMember(const char *name,const LogicDataObj &val) ;
	const LogicUserDefStruct *getUserDef() const;

	int GetArraySize() const;

	LogicDataObj  operator+(const LogicDataObj &r) const;
	LogicDataObj  operator-(const LogicDataObj &r) const;
	LogicDataObj  operator*(const LogicDataObj &r) const;
	LogicDataObj  operator/(const LogicDataObj &r) const;

	LogicDataObj  operator+(const char *text) const ;

	LogicDataObj & operator+=(const LogicDataObj &r);
	LogicDataObj & operator+=(const char *text);

	LogicDataObj operator[](int n);
	
	bool TransferType(DBL_ELEMENT_TYPE aimType);
	bool CheckValid() const;
	bool CheckArray(int index) const;

	void ConvertEncode(int from, int to);
	int Print(logic_print, void *pf) const;
	int Sprint(char *buf, size_t size) const;
	int toStdString(std::string &outstr) const;

	DBL_ELEMENT_TYPE GetDataType() const;
	DBL_ELEMENT_TYPE GetArrayType() const;

	bool BitOperateBin(eBitOperate opType, NDUINT8 opVal);
	bool setOutAsJson(bool bIsJson = true);
	static bool setOutHex(bool isHex = true);
	static bool setOutLua(bool isLua);

protected:
	LogicDataObj _attrMathAdd(const LogicDataObj &leftval)const ;
	LogicDataObj _attrMathSub(const LogicDataObj &leftval)const ;
	LogicDataObj _attrMathMul(const LogicDataObj &leftval)const ;
	LogicDataObj _attrMathDiv(const LogicDataObj &leftval)const ;

	LogicDataObj _arrayMathAdd(const LogicDataObj &leftval)const;
	LogicDataObj _arrayMathSub(const LogicDataObj &leftval)const;
	LogicDataObj _arrayMathMul(const LogicDataObj &leftval)const;
	LogicDataObj _arrayMathDiv(const LogicDataObj &leftval)const;

	void Destroy();
	void init() ;
	void _copy(const LogicDataObj &r);
	int _writeEmptyStream(char *streamBuf, int streamByteOrder)const;
	
	NDUINT8 m_ele_type;
	NDUINT8 m_sub_type;
	NDUINT8 m_outAsJson:1;
	NDUINT8 m_dataOwner:1; // false needn't release data 

	dbl_element_base *m_data;
	dbl_element_base m_dataOwn;

	static NDUINT8 s_bOutHex;
	static NDUINT8 s_bOutLua;
};


LOGIC_PARSER_API int dbl_destroy_data(dbl_element_base *buf, DBL_ELEMENT_TYPE etype, DBL_ELEMENT_TYPE sub_etype);
LOGIC_PARSER_API int dbl_build_from_text(dbl_element_base *buf, const char *in_data, DBL_ELEMENT_TYPE etype, DBL_ELEMENT_TYPE sub_etype);
LOGIC_PARSER_API int dbl_data_copy(const dbl_element_base *input, dbl_element_base *output, DBL_ELEMENT_TYPE etype, DBL_ELEMENT_TYPE sub_etype);

typedef size_t (*dbl_stream_fread)( void * _DstBuf,  size_t _ElementSize,  size_t _Count,  FILE * _File);
typedef size_t(*dbl_stream_fwrite)(const void * _DstBuf, size_t _ElementSize, size_t _Count, FILE * _File);

LOGIC_PARSER_API int dbl_data_2streamfile(dbl_element_base *buf, DBL_ELEMENT_TYPE etype, DBL_ELEMENT_TYPE sub_etype, FILE*pf, dbl_stream_fwrite writefunc,  bool changeByteOrder = false);
LOGIC_PARSER_API int dbl_read_streamfile(dbl_element_base *indata, DBL_ELEMENT_TYPE etype, DBL_ELEMENT_TYPE sub_etype, FILE*pf, dbl_stream_fread readfunc, bool changeByteOrder = false);

LOGIC_PARSER_API int dbl_read_buffer(dbl_element_base *data, DBL_ELEMENT_TYPE etype, DBL_ELEMENT_TYPE sub_etype, char *buf, bool changeByteOrder=false);
LOGIC_PARSER_API int dbl_write_buffer(dbl_element_base *data, DBL_ELEMENT_TYPE etype, DBL_ELEMENT_TYPE sub_etype, char *buf, bool changeByteOrder = false);


#endif
