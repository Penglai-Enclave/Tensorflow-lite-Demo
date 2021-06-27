extern "C" void eapp_print(const char* s, ...);

typedef long unsigned int size_t;
// extern "C" void *malloc(size_t);
// extern "C" void free(void *);

// void operator delete(void* p) {
//     eapp_print("delete stub.\n");
// 	free(p);
// }

// void *operator new(size_t size) {
//     eapp_print("new stub.\n");
// 	return malloc(size);
// }

// void *operator new[](size_t size) {
//     eapp_print("new[] stub.\n");
// 	return malloc(size);
// }

// void operator delete[](void* p) {
//     eapp_print("delete[] stub.\n");
// 	free(p);
// }

// void operator delete(void* p, size_t) {
//     eapp_print("delete2 stub.\n");
// 	free(p);
// }

void __calloc(){
    eapp_print("__calloc stub.\n");
}

struct locale_t
{
    /* data */
};

extern "C" unsigned long long strtoull_l (const char *__restrict s, char **__restrict ptr, int base, locale_t loc){
    eapp_print("strtoull_l stub.\n");
}

extern "C" void *strtoll_l(void){
    eapp_print("strtoll_l stub.\n");
}

extern "C" void *__libc_fatal(void){
    eapp_print("__libc_fatal stub.\n");
}

extern "C" void *__libc_longjmp(void){
    eapp_print("__libc_longjmp stub.\n");
}

extern "C" void *calloc(long unsigned int, long unsigned int){
    eapp_print("calloc stub.\n");
}
