
typedef struct textfrog_s {
	int env ;
} * textfrog ;

# ifdef __cplusplus
extern "C" {
	# endif

	# define TFG_API
	TFG_API textfrog tfg_init ( int debug ) ;
	TFG_API int tfg_include ( textfrog tfg , const char * fn ) ;
	TFG_API long * tfg_call ( textfrog tfg , const char * func , int argc , ... ) ;
	TFG_API long * tfg_call2 ( textfrog tfg , const char * func , const char * paramSig , ... ) ;
	TFG_API long * tfg_calljava ( textfrog tfg , const char * clz , const char * func , const char * paramSig , ... ) ;
	TFG_API void tfg_flushLog ( ) ;
	TFG_API int detectJvm ( ) ;
	# ifdef __cplusplus
}
# endif

