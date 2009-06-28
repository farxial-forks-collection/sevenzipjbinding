#include "SevenZipJBinding.h"

#include "JNITools.h"
#include "CPPToJavaInStream.h"


STDMETHODIMP CPPToJavaInStream::Seek(Int64 offset, UInt32 seekOrigin, UInt64 *newPosition)
{
    TRACE_OBJECT_CALL("Seek");

	TRACE2("SEEK(offset=%i, origin=%i)", (int)offset, (int)seekOrigin);

    JNIInstance jniInstance(_nativeMethodContext);
    JNIEnv * env = jniInstance.GetEnv();

//	jlongArray newPositionArray = env->NewLongArray(1);
//	FATALIF(newPositionArray == NULL, "Out of local resource of out of memory: newPositionArray == NULL");

    jniInstance.PrepareCall();
	jlong returnedNewPosition = env->CallIntMethod(_javaImplementation, _seekMethodID, (jlong)offset, (jint)seekOrigin);

	if (jniInstance.IsExceptionOccurs())
	{
		return S_FALSE;
	}

	if (newPosition) {
		*newPosition = (UInt64)returnedNewPosition;
	}

	TRACE1("SEEK: New Pos: %i", (int)(UInt64)returnedNewPosition)

//	env->ReleaseLongArrayElements(newPositionArray, newPositionArrayData, JNI_ABORT);
//	env->DeleteLocalRef(newPositionArray);

	return S_OK;
}


