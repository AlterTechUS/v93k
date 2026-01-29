#ifndef __DDR4_OPERATIONS_INC__
#define __DDR4_OPERATIONS_INC__

//////////////////////////////////////////////////
// DDR4Operations.inc
// this file defines operations for a DDR4 device
//////////////////////////////////////////////////

#ODT control
#define   ODT_ON    1
#define   ODT_OFF   0

#DM control
#define   UNMASK    0x0
#define   MASK      0xF

##################################################################
# Deselect operation for n tCK cyles 
##################################################################        
Idle(n)
{
    for(i=0; i<n; i+=1){
        Deselect(); 
    }
}

##################################################################
# Deselect operation with odt for n tCK cyles 
##################################################################        
Idle_OdtON(n)
{
    for(i=0; i<n; i+=1){
        Deselect_OdtON(); 
    }
}

##################################################################
# Deselect operation for n tCK cyles at half rate
##################################################################        
Idle_HR(n)
{
    for(i=0; i<n; i+=1){
        Deselect_HR(); 
    }
}

##################################################################
# Deselect operation for n tCK cyles at full rate
##################################################################        
Idle_FR(n)
{
    for(i=0; i<n; i+=1){
        Deselect_FR(); 
    }
}

##################################################################
# Deselect operation
##################################################################        
Deselect()
{
    CLK();  CTRL(1,ODT_OFF);  DES();   DBUS_IDLE();  CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Deselect operation with odt
##################################################################        
Deselect_OdtON()
{
    CLK();  CTRL(1,ODT_ON);  DES();   DBUS_IDLE();   CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Deselect operation at half rate
##################################################################        
Deselect_HR()
{
    CLK_HR();  CTRL_HR(1,ODT_OFF);  DES_HR();   DBUS_IDLE_HR();  CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Deselect operation at full rate
##################################################################        
Deselect_FR()
{
    CLK_FR();  CTRL_FR(1,ODT_OFF);  DES_FR();   DBUS_IDLE_FR();  CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Mode register setting
##################################################################        
ModeRegister(regNo, opCode)
{
    CLK();  CTRL(1,ODT_OFF);  MRS(regNo, opCode);   DBUS_IDLE();  CNBUS_SPECIFY(0,0,0);
}

##################################################################
# ZQCcal command
##################################################################        
ZQCalibration()
{  
    CLK(); ACBUS_SPECIFY(1,0,1,1,1,0,0xFFFFFFFF,0xFFFFFFFFF); DBUS_IDLE(); CTRL(1,ODT_OFF);  CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Activate the row in given bank
##################################################################        
Activate(row, bank)
{
    CLK();  CTRL(1,ODT_OFF);  ACT(row, bank);   DBUS_IDLE();  CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Precharge the given bank and close the open row
##################################################################        
Precharge(bank)
{
    CLK();  CTRL(1,ODT_OFF);  PRE(bank);   DBUS_IDLE();  CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Precharge all banks
##################################################################        
PrechargeA()
{
    CLK();  CTRL(1,ODT_OFF);  PREA();   DBUS_IDLE();  CNBUS_SPECIFY(0,0,0);
}


##################################################################
# Write preamble for one tCK, used before Write()
##################################################################        
Write_Preamb()
{
    CLK();  CTRL(1,ODT_OFF);  DES();  DBUS_PREAMB();   CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Write a burst of BL=8 
##################################################################        
Write(row, col, bank, mask, dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8)
{
    CLK();  CTRL(1,ODT_OFF);  WR(col, bank); DBUS_WR8(dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8,mask,row,col,bank);   CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                        CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                        CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                        CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                        CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Write a burst of BL=8 wiht odt
##################################################################        
Write_ODT(row, col, bank, mask, dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8)
{
    CLK();  CTRL(1,ODT_ON);  WR(col, bank); DBUS_WR8(dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8,mask,row,col,bank);    CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_ON);  DES();                                                                         CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_ON);  DES();                                                                         CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_ON);  DES();                                                                         CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_ON);  DES();                                                                         CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Write a burst of BL=8  with auto-precharge 
##################################################################        
WriteAP(row, col, bank, mask, dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8)
{
    CLK();  CTRL(1,ODT_OFF);  WRA(col, bank); DBUS_WR8(dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8,mask,row,col,bank);  CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                        CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                        CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                        CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                        CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Read a burst of BL=8 
##################################################################        
Read(row, col, bank, dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8)
{
    CLK();  CTRL(1,ODT_OFF);  RD(col, bank); DBUS_RD8(dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8,row,col,bank);   CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                   CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                   CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                   CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                   CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Read a burst of BL=8 
##################################################################        
Read_XXEXXXEX(row, col, bank, dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8)
{
    CLK();  CTRL(1,ODT_OFF);  RD(col, bank); DBUS_RD8_XXEXXXEX(dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8,row,col,bank);   CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                            CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                            CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                            CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                            CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Read2 a burst of BL=8 
##################################################################        
Read2(row, col, bank, dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8)
{
    CLK();  CTRL(1,ODT_OFF);  RD(col, bank); DBUS_RD8only(dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8,row,col,bank);   CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                       CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                       CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                       CNBUS_SPECIFY(0,0,0);
}   


##################################################################
# Read a burst of BL=8 with auto-precharge 
##################################################################        
ReadAP(row, col, bank, dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8)
{
    CLK();  CTRL(1,ODT_OFF);  RDA(col, bank); DBUS_RD8(dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8,row,col,bank);  CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                   CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                   CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                   CNBUS_SPECIFY(0,0,0);
    CLK();  CTRL(1,ODT_OFF);  DES();                                                                   CNBUS_SPECIFY(0,0,0);
}

##################################################################
# Refresh
##################################################################        
Refresh()
{
    CLK(); CTRL(1,ODT_OFF);  REF(); DBUS_IDLE();   CNBUS_SPECIFY(0,0,0);
}

#endif //__DDR4_OPERATIONS_INC__ 
