/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <math.h>
/* Example/Board Header files */
#include "Board.h"

#define SET(x,y) x |= (1 << y)
#define CLEAR(x,y) x &= ~(1<< y)
#define READ(x,y) ((0u == (x & (1<<y)))?0u:1u)
#define BIT(x) (1UL << (x))
#define TOGGLE(x,y) (x ^= (1<<y))
#define REG_STATUS 0x3FFF
#define TASKSTACKSIZE       1024
#define MLX90632_REG_STATUS   0x3fff
#define SLAVE_ADDRESS 0x3A
#define MLX90632_STAT_DATA_RDY   BIT(0)
#define MLX90632_REG_STATUS   0x3fff
#define MLX90632_ADDR_RAM   0x4000
#define EE_I2C_ADDRESS 0x24D5
#define REG_CONTROL_ADD 0x3001 //Control register, measurement mode
#define RAM_4 0x4003
#define RAM_5 0x4004
#define RAM_6 0x4005
#define RAM_7 0x4006
#define RAM_8 0x4007
#define RAM_9 0x4008
double temp = 25;
//Task handler
Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];
//I2C handler
uint16_t i2cReadReg16(uint8_t *wBuff);
uint32_t i2cReadReg32(uint8_t *wBuff);

Void i2cWriteReg32(uint8_t *wBuff);
struct EEPROM_Obj eeprom32[17];
struct EEPROM_Obj_16 eeprom16[5];
double calculateTempAmbient();
double AMB();
uint16_t readRamValue(uint16_t ram);
double calculateObjectTemp(double TOdut, uint16_t ram1, uint16_t ram2);
double TAdut();
double Sto(uint16_t ram1, uint16_t ram2);
void getTemp();


typedef struct EEPROM_Obj {
   uint16_t  address;
   int pow;
   uint32_t data;
   double calculatedValue;
} EEPROM_Obj;

typedef struct EEPROM_Obj_16 {
   uint16_t  address;
   int pow;
   uint32_t data;
   double calculatedValue;
} EEPROM_Obj_16;

unsigned int    i;
   //uint16_t        temperature;
   //uint8_t         txBuffer[2];
   uint16_t        txBuffer;
   uint16_t        rxBuffer;
   I2C_Handle      i2c;
   I2C_Params      i2cParams;

Void i2cWriteReg32(uint8_t *wBuff){
    I2C_Transaction i2cTransaction;
    i2cTransaction.slaveAddress = SLAVE_ADDRESS; /* 7-bit peripheral slave address */
    i2cTransaction.writeBuf = wBuff; /* Buffer to be written */
    i2cTransaction.writeCount = 4; /* Number of bytes to be written */
    i2cTransaction.readBuf = NULL; /* Buffer to be read */
    i2cTransaction.readCount = 0; /* Number of bytes to be read */
    I2C_transfer(i2c, &i2cTransaction); /* Perform I2C transfer */
}

uint32_t i2cReadReg32(uint8_t *wBuff){
    I2C_Transaction i2cTransaction;
    uint8_t readBuff[4];
    i2cTransaction.slaveAddress = SLAVE_ADDRESS; /* 7-bit peripheral slave address */
    i2cTransaction.writeBuf = wBuff; /* Buffer to be written */
    i2cTransaction.writeCount = 2; /* Number of bytes to be written */
    i2cTransaction.readBuf = readBuff; /* Buffer to be read */
    i2cTransaction.readCount = 4; /* Number of bytes to be read */
    //i2cTransaction.arg = WRITE_ID; /* I2C_transfers will always return successful */
    I2C_transfer(i2c, &i2cTransaction); /* Perform I2C transfer */
    uint8_t msb0 = readBuff[0];
    uint8_t lsb0 = readBuff[1];
    uint8_t msb1 = readBuff[2];
    uint8_t lsb1 = readBuff[3];
    uint16_t lower = (uint16_t)msb0 << 8 | lsb0;
    uint16_t upper = (uint16_t)msb1 << 8 | lsb1;
    uint32_t out = (uint32_t)upper << 16 | lower;
    return out;
 }

uint16_t i2cReadReg16(uint8_t *writeBuff){
    uint8_t readBuff[2];
    I2C_Transaction i2cTransaction;
    i2cTransaction.slaveAddress = SLAVE_ADDRESS;
    i2cTransaction.writeBuf = writeBuff;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = readBuff;
    i2cTransaction.readCount = 2;
    if(I2C_transfer(i2c, &i2cTransaction));
    uint8_t msb = (uint8_t) readBuff[0];
    uint8_t lsb = (uint8_t) readBuff[1];
    uint16_t out = (uint16_t) msb << 8 | lsb;
    return out;
}

EEPROM_Obj_16 read16EPROMData(EEPROM_Obj_16 obj){
        uint8_t write16[2];
        if(obj.pow != 999){
            write16[0] = obj.address >> 8;
            write16[1] = obj.address & 0xFF;
            obj.data = i2cReadReg16(write16);
            obj.calculatedValue = (double)obj.data * pow(2, obj.pow);
            System_printf("%d\n", obj.calculatedValue);
            return obj;
        }else{
            write16[0] = obj.address >> 8;
            write16[1] = obj.address & 0xFF;
            obj.calculatedValue = NULL;
            obj.data = i2cReadReg16(write16);
           // System_printf("%d\n", obj.data);
            return obj;
        }
}

EEPROM_Obj read32EPROMData(EEPROM_Obj obj){
        uint8_t write16[2];
        if(obj.pow != 999){
            write16[0] = obj.address >> 8;
            write16[1] = obj.address & 0xFF;
            obj.data = i2cReadReg32(write16);
            obj.calculatedValue = (double)obj.data * pow(2, obj.pow);
            System_printf("%d\n", obj.calculatedValue);
            return obj;
        }else{
            write16[0] = obj.address >> 8;
            write16[1] = obj.address & 0xFF;
            obj.data = i2cReadReg32(write16);
            obj.calculatedValue = NULL;
           // System_printf("%d\n", obj.data);
            return obj;
        }
}

Void init(){
    //32 bit structs
    struct EEPROM_Obj EE_P_R; //a
    EE_P_R.address = 0x240C;
    EE_P_R.pow = -8;
    struct EEPROM_Obj EE_P_G;//a
    EE_P_G.address =  0x240E;
    EE_P_G.pow = -20;
    struct EEPROM_Obj EE_P_T;//a
    EE_P_T.address = 0x2410;
    EE_P_T.pow = -44;
    struct EEPROM_Obj EE_P_O;//a
    EE_P_O.address = 0x2412;
    EE_P_O.pow = -8;
    struct EEPROM_Obj EE_Aa;//a
    EE_Aa.address = 0x2414;
    EE_Aa.pow = 999;
    struct EEPROM_Obj EE_Ab;//a
    EE_Ab.address = 0x2416;
    EE_Ab.pow = 999;
    struct EEPROM_Obj EE_Ba;//a
    EE_Ba.address = 0x2418;
    EE_Ba.pow = 999;
    struct EEPROM_Obj EE_Bb;//a
    EE_Bb.address = 0x241A;
    EE_Bb.pow = 999;
    struct EEPROM_Obj EE_Ca;//a
    EE_Ca.address = 0x241C;
    EE_Ca.pow = 999;
    struct EEPROM_Obj EE_Cb;//a
    EE_Cb.address = 0x241E;
    EE_Ca.pow = 999;
    struct EEPROM_Obj EE_Da;//a
    EE_Da.address = 0x2420;
    EE_Da.pow = 999;
    struct EEPROM_Obj EE_Db;//a
    EE_Db.address = 0x2422;
    EE_Db.pow = 999;
    struct EEPROM_Obj EE_Ea;//a
    EE_Ea.address = 0x2424;
    EE_Ea.pow = -16;
    struct EEPROM_Obj EE_Eb;//a
    EE_Eb.address = 0x2426;
    EE_Eb.pow = -8;
    struct EEPROM_Obj EE_Fa;//a
    EE_Fa.address = 0x2428;
    EE_Fa.pow = -46;
    struct EEPROM_Obj EE_Fb;//a
    EE_Fb.address = 0x242A;
    EE_Fb.pow = -36;
    struct EEPROM_Obj EE_Ga;//a
    EE_Ga.address = 0x242C;
    EE_Ga.pow = -36;
    //16bit strucks
    struct EEPROM_Obj_16 EE_Gb;
    EE_Gb.address = 0x242E;
    EE_Gb.pow = -10;
    struct EEPROM_Obj_16 EE_Ka;
    EE_Ka.address = 0x242F;
    EE_Ka.pow= -10;
    struct EEPROM_Obj_16 EE_Kb;
    EE_Kb.address = 0x2430;
    EE_Kb.pow = 999;
    struct EEPROM_Obj_16 EE_Ha;
    EE_Ha.address = 0x2481;
    EE_Ha.pow = -14;
    struct EEPROM_Obj_16 EE_Hb;
    EE_Hb.address = 0x2481;
    EE_Hb.pow = -14;
    eeprom32[0] = EE_P_R;
    eeprom32[1] = EE_P_G;
    eeprom32[2] = EE_P_T;
    eeprom32[3] = EE_P_O;
    eeprom32[4] = EE_Aa;
    eeprom32[5] = EE_Ab;
    eeprom32[6] = EE_Ba;
    eeprom32[7] = EE_Bb;
    eeprom32[8] = EE_Ca;
    eeprom32[9] = EE_Cb;
    eeprom32[10] = EE_Da;
    eeprom32[11] = EE_Db;
    eeprom32[12] = EE_Ea;
    eeprom32[13] = EE_Eb;
    eeprom32[14] = EE_Fa;
    eeprom32[15] = EE_Fb;
    eeprom32[16] = EE_Ga;
    //16bit
    eeprom16[0] = EE_Gb;
    eeprom16[1] = EE_Ka;
    eeprom16[2] = EE_Kb;
    eeprom16[3] = EE_Ha;
    eeprom16[4] = EE_Hb;
    //buffers for init
    uint8_t write16[2];
    uint8_t write32[4];
    uint16_t checkAddress;
    write16[0] = (EE_I2C_ADDRESS >> 8);
    write16[1] = (EE_I2C_ADDRESS & 0xff);
    checkAddress = i2cReadReg16(write16);

    if(checkAddress != (SLAVE_ADDRESS >> 1)){
        System_printf("Address did not match\n");
    } else {
        System_printf("Address matched, I2C communication active\n");
    }
    //Read the status register
    uint16_t status;
    write16[0] = (MLX90632_REG_STATUS >> 8);//
    write16[1] = (MLX90632_REG_STATUS & 0xff);
    status = i2cReadReg16(write16);
    System_printf("%x\n", status);
    //Getting the control registers value
    uint16_t regControlData;
    write16[0] = (REG_CONTROL_ADD >> 8);//MSByte address
    write16[1] = (REG_CONTROL_ADD & 0xff);//LSByte address
    regControlData = i2cReadReg16(write16);//Control Register
    System_printf("%x\n", regControlData);

    int i;
    for(i = 0; i < 17; i++){
        eeprom32[i] = read32EPROMData(eeprom32[i]);
        if(i < 5){
            eeprom16[i] = read16EPROMData(eeprom16[i]);
        }
    }
    CLEAR(regControlData, 2);//reset the previously set REG_CONTROL bit
    CLEAR(regControlData, 1);
    SET(regControlData, 2); //wake up the device now, and put into continuous mode
    SET(regControlData, 1);
    write32[0] = (REG_CONTROL_ADD >> 8);  //MSByte Address
    write32[1] = (REG_CONTROL_ADD & 0xff); //LSByte Address
    write32[2] = (regControlData >> 8); //MSByte Data
    write32[3] = (regControlData & 0xFF); //LSByte Data
    i2cWriteReg32(write32);
}
/*
 * The sequence should look like the following:
1 Write new_data = 0
2 Check when new_data = 1
3 Read cycle_pos to get measurement pointer
4 If cycle_pos = 1
5 Calculate TA and TO based on RAM_4, RAM_5, RAM_6, RAM_9
6 If cycle_pos = 2
7 Calculate TA and TO based on RAM_7, RAM_8, RAM_6, RAM_9
8 Return to top
 */
void getTemp(){
    //locals
    uint8_t write16[2];
    uint8_t write32[4];
    uint16_t status;
    int i;

    while(1){
    //1. Write new data
    write16[0] = (MLX90632_REG_STATUS >> 8);//
    write16[1] = (MLX90632_REG_STATUS & 0xff);
    status = i2cReadReg16(write16);
    if(READ(status, 0) == 1){
        CLEAR(status, 0);
           write32[0] = (REG_STATUS >> 8);  //MSByte Address
           write32[1] = (REG_STATUS & 0xff); //LSByte Address
           write32[2] = (status >> 8); //MSByte Data
           write32[3] = (status & 0xFF); //LSByte Data
           i2cWriteReg32(write32);
    }
    while(1){
        write16[0] = (MLX90632_REG_STATUS >> 8);//
        write16[1] = (MLX90632_REG_STATUS & 0xff);
        status = i2cReadReg16(write16);
        if(READ(status,0) == 1){
            break;
        }
    }

    //3. where is the cycle
    uint16_t pos;
    write16[0] = (MLX90632_REG_STATUS >> 8);
    write16[1] = (MLX90632_REG_STATUS & 0xff);
    status = i2cReadReg16(write16);
    pos = status >> 2;
    pos &= 0x1F;//Position of cycle

   //4.
    if(pos == 1){
        uint16_t ram1 = readRamValue(RAM_4);
        uint16_t ram2 = readRamValue(RAM_5);
        temp = calculateObjectTemp(temp, ram1, ram2);
    }
    if(pos == 2){
        uint16_t ram1 = readRamValue(RAM_7);
        uint16_t ram2 = readRamValue(RAM_8);
        temp = calculateObjectTemp(temp, ram1, ram2);
    }
    Task_sleep(600);
    }
}

double Sto(uint16_t ram1, uint16_t ram2){
    double s = (double)((ram1 + ram2) / 2.0);
    uint16_t ram9 = readRamValue(RAM_9);
    uint16_t ram6 = readRamValue(RAM_6);
    double Ka = eeprom16[1].calculatedValue;
    double Vrto = ram9 + (Ka * (ram6 / 12.0));
    double Sto = ((s/12.0) / (Vrto * pow(2,19)));
    return Sto;
}

double TAdut(){
    return (((AMB() - (eeprom32[13].calculatedValue  )) / (eeprom32[12].calculatedValue) ) + 25);
}

double calculateObjectTemp(double TOdut, uint16_t ram1, uint16_t ram2){
    int emissivity = 1;
    double Fa = (eeprom32[14].calculatedValue);
    double Fb = -(eeprom32[15].calculatedValue);
    double Ga = -(eeprom32[16].calculatedValue);
    double Ha = (eeprom16[3].calculatedValue);
    double Hb = (eeprom16[4].calculatedValue);
    double TAo = 25;
    double TAdutt = TAdut();
    double Ea = eeprom32[12].calculatedValue;
    double Eb = eeprom32[13].calculatedValue;
   double Tak = pow((TAdutt + 273.15), 4);
    double TOo = 25;
    double Sto_ = Sto(ram1, ram2);
    double objectValue = ((Sto_ / (emissivity * Fa * Ha * (1 + Ga * (TOdut - TOo) + Fb * (TAdutt - TAo)))) + Tak);
    double TO = ((pow(objectValue, (1.0 / 4.0))) - 273.15 - Hb);
    temp = TO;
    return TO;
}

double AMB(){
    uint16_t ram9 = readRamValue(RAM_9);
    uint16_t ram6 = readRamValue(RAM_6);
    double VR = (ram9 + eeprom16[0].calculatedValue * (ram6/12));
    return ((ram6/12) / VR * pow(2,19));
}

double calculateTempAmbient(){
   double amb = AMB();
   return (eeprom32[3].calculatedValue + ((amb - eeprom32[0].calculatedValue)/(eeprom32[1].calculatedValue)) + eeprom32[2].calculatedValue * pow((amb - eeprom32[0].calculatedValue),2));
}

uint16_t readRamValue(uint16_t ram){
    uint8_t write16[2];
    write16[0] = ram >> 8;
    write16[1] = ram & 0xFF;
    return i2cReadReg16(write16);
}

Void taskFxn(UArg arg0, UArg arg1)
{
    init();
    getTemp();
    while(1){
    }
}


int main(void)
{
    Task_Params taskParams;

    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initI2C();
    /* Construct tmp006 Task thread */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)taskFxn, &taskParams, NULL);
    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2cParams.transferMode = I2C_MODE_BLOCKING;
    i2c = I2C_open(Board_I2C2, &i2cParams);
    if (i2c == NULL) {
        System_abort("Error Initializing I2C\n");
    }
    else {
        System_printf("I2C Initialized!\n");
    }
    /* Start BIOS */
    BIOS_start();

    return (0);
}
