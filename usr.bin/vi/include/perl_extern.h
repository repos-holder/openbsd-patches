
int perl_end(GS *);
int perl_init(SCR *);
int perl_screen_end(SCR*);
int perl_ex_perl(SCR*, CHAR_T *, size_t, recno_t, recno_t);
int perl_ex_perldo(SCR*, CHAR_T *, size_t, recno_t, recno_t);
#ifdef USE_SFIO
Sfdisc_t* sfdcnewnvi(SCR*);
#endif
