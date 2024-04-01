#pragma once


/// @brief All languages in our software
typedef enum
{
  LANG_INFO    =0,
  LANG_SIMPLIFIED =1,
  LANG_DUTCH   =2,
  LANG_ENGLISH =3,
  LANG_GERMAN  =4,
  LANG_INDO    =5,
  LANG_TRADITIONAL =6,
  LANG_NEXT    =7,
  LANG_MAX     =LANG_NEXT,
  LANG_CURRENT =-1
} Taal;

extern Taal EuroLang, PrintLang;

extern void ChangeLanguage(Taal language);
extern void setLanguage(Taal language);
extern Taal ChangeNextLanguage(Taal language);
extern void Language(Taal *l, Taal language);
extern Taal getLanguage();
extern bool isChinese();
extern int NumberLanguages();
