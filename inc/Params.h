#ifndef INC_PARAMS_H_
#define INC_PARAMS_H_

#include "APDB.h"

/* Instelbare parameter met bereik. Id komt overeen met ConfigGroup uit can_messages. */
typedef struct {
    ubyte1  Id;
    ubyte2  Default;
    ubyte2  Min;
    ubyte2  Max;
    ubyte2 *Value;
} Param_t;

/* Zet door de CAN-handler; de hoofdlus schrijft het EEPROM zodra de vlag gezet is. */
extern bool ParamsSavePending;

void ParamsLoadDefaults(void);
void ParamsValidate(void);
const Param_t *ParamsFind(ubyte1 Id);
bool ParamsSet(ubyte1 Id, ubyte2 NewValue);
void ParamsApply(void);

#endif /* INC_PARAMS_H_ */
