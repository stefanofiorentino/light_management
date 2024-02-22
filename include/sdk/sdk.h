#ifdef __cplusplus
extern "C"
{
#endif

  // defined
  void commissioningInit(void* context);
  void startCommissioning(const char* message);

  // to be defined
  void onCommissioningInfo(const char*, const char*, void* context);

#ifdef __cplusplus
}
#endif
