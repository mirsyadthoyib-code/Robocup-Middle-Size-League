
int32_t rotaryCount;
int32_t lastCount;
int16_t error;
int16_t errorDua;
int32_t rpm;
int32_t rpmDua;
int16_t setPointSatu;
int16_t setPointDua;
int32_t output;
int32_t outputDua;
int32_t speed;
int16_t prevError;
int16_t derivError;
int16_t intError;
int16_t prevErrorDua;
int16_t derivErrorDua;
int16_t intErrorDua;

void InitializeTimer();
void EnableTimerInterrupt();
void TIM2_IRQHandler();
void TIM5_IRQHandler();
void init_tim();


