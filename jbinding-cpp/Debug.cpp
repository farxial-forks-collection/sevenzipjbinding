
#include <map>
#include "SevenZipJBinding.h"

#ifdef TRACE_OBJECTS_ON

#ifdef COMPRESS_MT
#include "Windows/Synchronization.h"
#endif

#ifdef COMPRESS_MT
	#define ENTER_CRITICAL_SECTION   {g_criticalSection.Enter();}
	#define LEAVE_CRITICAL_SECTION   {g_criticalSection.Leave();}
#else
	#define ENTER_CRITICAL_SECTION   {}
	#define LEAVE_CRITICAL_SECTION   {}
#endif

#ifdef COMPRESS_MT
    NWindows::NSynchronization::CCriticalSection g_criticalSection;
#endif

using namespace std;

struct ClassInfo
{
    const char * _classname;
    void * _thiz;
};

map<void *, ClassInfo *> g_classes_map;

void TracePrintObjects()
{
	ENTER_CRITICAL_SECTION

	map<void *, ClassInfo *>::const_iterator i = g_classes_map.begin();
    _TRACE("Objects alive:\n")
#ifdef TRACE_ON
    int count = 1;
    for (; i != g_classes_map.end(); i++)
    {
        _TRACE3("> %3i %s (this: 0x%08X)\n", count++, (*i).second->_classname, (size_t)(*i).second->_thiz)
    }
#endif
    LEAVE_CRITICAL_SECTION
}

void TracePrintObjectsUsingPrintf()
{
	ENTER_CRITICAL_SECTION

	map<void *, ClassInfo *>::const_iterator i = g_classes_map.begin();
    printf("Objects alive:\n");

    int count = 1;
    for (; i != g_classes_map.end(); i++)
    {
        printf("> %3i %s (this: 0x%08X)\n", count++, (*i).second->_classname, (size_t)(*i).second->_thiz);
    }
    fflush(stdout);

    LEAVE_CRITICAL_SECTION
}
void TraceObjectCreation(const char * classname, void * thiz)
{
	ENTER_CRITICAL_SECTION
	int found = g_classes_map.find(thiz) == g_classes_map.end();
	LEAVE_CRITICAL_SECTION

	if (found)
    {
        ClassInfo * classInfo = new ClassInfo();
        classInfo->_classname = classname;
        classInfo->_thiz = thiz;

        ENTER_CRITICAL_SECTION
        g_classes_map[thiz] = classInfo;
		_TRACE3("++++++++ %s (this: 0x%08X) [classes alive: %i]\n", classInfo->_classname, (size_t)classInfo->_thiz, g_classes_map.size())
		LEAVE_CRITICAL_SECTION

    }
    else
    {
        ENTER_CRITICAL_SECTION
        g_classes_map[thiz]->_classname = classname;
		LEAVE_CRITICAL_SECTION

		_TRACE2("KNOWN AS %s (this: 0x%08X)\n", classname, (size_t)thiz)
    }

}
void TraceObjectDestruction(void * thiz)
{
	ENTER_CRITICAL_SECTION
	int found = g_classes_map.find(thiz) == g_classes_map.end();
    LEAVE_CRITICAL_SECTION

    if (found)
    {
        fatal("TraceObjectDestruction(): destructor called for unknown this=0x%08X", (size_t)thiz);
    }

#ifdef TRACE_ON
	ENTER_CRITICAL_SECTION
    ClassInfo * classInfo = g_classes_map[thiz];
    LEAVE_CRITICAL_SECTION
#endif

	ENTER_CRITICAL_SECTION
    g_classes_map.erase(thiz);
    _TRACE3("~~~ %s (this: 0x%08X) [classes alive: %i]\n", classInfo->_classname, (size_t)classInfo->_thiz, g_classes_map.size())
    LEAVE_CRITICAL_SECTION

    TracePrintObjects();

}

void TraceObjectCall(void * thiz, const char * methodname)
{
	ENTER_CRITICAL_SECTION
	int found = g_classes_map.find(thiz) == g_classes_map.end();
    LEAVE_CRITICAL_SECTION

    if (found)
    {
        fatal("Object call for dead object. Method name: %s, this: 0x%08X", methodname, (size_t)thiz);
    }

#ifdef TRACE_ON
	ENTER_CRITICAL_SECTION
    ClassInfo * classInfo = g_classes_map[thiz];
    _TRACE4("-> %s::%s%s (this: 0x%08X)\n",classInfo->_classname, methodname,
            (strchr(methodname, '(') == NULL ? "(...)" : ""), (size_t)thiz);
    LEAVE_CRITICAL_SECTION
#endif

}

void TraceObjectEnsureDestruction(void * thiz)
{
	ENTER_CRITICAL_SECTION
	int found = g_classes_map.find(thiz) != g_classes_map.end();
    LEAVE_CRITICAL_SECTION

    if (found)
    {
    	ENTER_CRITICAL_SECTION
        ClassInfo * classInfo = g_classes_map[thiz];
		LEAVE_CRITICAL_SECTION
        fatal("Objcet %s (this: 0x%08X) wasn't destroyed as expected\n",classInfo->_classname, (size_t)thiz);
    }

}
#endif // TRACE_OBJECTS_ON