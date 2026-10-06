#include "params.h"
#include "sysconf.h"

bool ParamsSavePending = FALSE;

/* Id's zijn gelijk aan ConfigGroup in can_messages.h */
#define PARAM_ID_PLANTSPACING 3

/* Een nieuwe instelling toevoegen = een regel hier + een veld in CFG_t */
static const Param_t ParamTable[] =
{
    /* Id,                     Default, Min, Max,  Value (mm) */
    { PARAM_ID_PLANTSPACING,   600,     100, 2000, &cfg.plantSpacingMm },
};

#define PARAM_COUNT (sizeof(ParamTable) / sizeof(ParamTable[0]))

void ParamsLoadDefaults(void)
{
    ubyte1 Index;
    for (Index = 0; Index < PARAM_COUNT; Index++)
    {
        *ParamTable[Index].Value = ParamTable[Index].Default;
    }
}

/* Waarden uit het EEPROM die buiten het bereik vallen worden vervangen door de default */
void ParamsValidate(void)
{
    ubyte1 Index;
    for (Index = 0; Index < PARAM_COUNT; Index++)
    {
        const Param_t *Param = &ParamTable[Index];
        if (*Param->Value < Param->Min || *Param->Value > Param->Max)
        {
            *Param->Value = Param->Default;
        }
    }
}

const Param_t *ParamsFind(ubyte1 Id)
{
    ubyte1 Index;
    for (Index = 0; Index < PARAM_COUNT; Index++)
    {
        if (ParamTable[Index].Id == Id)
        {
            return &ParamTable[Index];
        }
    }
    return 0;
}

/* Weigert onbekende parameters en waarden buiten min/max */
bool ParamsSet(ubyte1 Id, ubyte2 NewValue)
{
    const Param_t *Param = ParamsFind(Id);
    if (Param == 0 || NewValue < Param->Min || NewValue > Param->Max)
    {
        return FALSE;
    }

    if (*Param->Value != NewValue)
    {
        *Param->Value = NewValue;
        ParamsApply();
        ParamsSavePending = TRUE;
    }
    return TRUE;
}

/* Rekent de opgeslagen eenheden om naar de eenheden die de regeling gebruikt */
void ParamsApply(void)
{
    cfg.plantDistance = (float)cfg.plantSpacingMm / 1000.0f;
}
