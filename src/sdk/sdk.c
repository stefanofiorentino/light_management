#include <sdk/sdk.h>

static void* local_context;

void
commissioningInit(void* context)
{
  local_context = context;
}
void
startCommissioning(const char* message)
{
  // if(!parseMessage) return err;
  // if(!checkEmailValidity) return err;
  // if(!checkPasswordValidity) return err;
  onCommissioningInfo("extractedEmail", "extractedPassword", local_context);
}
