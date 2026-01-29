// basic DDR4 initialization routine 
// The device mode register settings and waiting times are defined in the apg file.
#include "../../device_defs/include/DDR4Operations.inc"

initialize() 
{	

    REFRESH_CTRL(STOP);

    ALL_ONES_ADDR = 0xFFFF;

    // After Power has been applied to the device wait for the device to stabilize.
    // RESETn low - for 200us; keep CKE low too
    for(i=0; i<tResetLow; i++)
    {
        CLK(); ACBUS_SPECIFY(0,0,1,0,0,0,ALL_ONES_ADDR,ALL_ONES_ADDR); DBUS_IDLE(); CTRL(0,ODT_OFF); CNBUS_SPECIFY(0,0,0);
    }

    // raise RESETn - but keep CKE low...for another 500us
    for(i=0; i<tCKELow; i++)
    {
        CLK(); ACBUS_SPECIFY(0,0,1,0,0,0,ALL_ONES_ADDR,ALL_ONES_ADDR); DBUS_IDLE(); CTRL(1,ODT_OFF); CNBUS_SPECIFY(0,0,0);
    }

    // MRS value setting
    MR0_VAL = 0x00000000; // 
    MR1_VAL = 0x00000000; // 
    MR2_VAL = 0x00000000; //
    MR3_VAL = 0x00000000; //
    MR4_VAL = 0x00000000; //
    MR5_VAL = 0x00000000; //
    MR6_VAL0= 0x00000000; //  base
    MR6_VAL1= 0x00000000; //  VrefDQ Training Entry
    MR6_VAL2= 0x00000000; //  VrefDQ Training NewValue set
    MR6_VAL3= 0x00000000; //  VrefDQ Training Exit

    if ( BL == 0 )          { MR0_VAL = MR0_VAL | 0x00000000; }  // 8 (Fixed)
    if ( BL == 1 )          { MR0_VAL = MR0_VAL | 0x00000001; }  // BC4 or 8 (on the fly)
    if ( BL == 2 )          { MR0_VAL = MR0_VAL | 0x00000002; }  // BC4 (Fixed)

    if ( BT == 0 )          { MR0_VAL = MR0_VAL | 0x00000000; }  // Sequential
    if ( BT == 1 )          { MR0_VAL = MR0_VAL | 0x00000008; }  // Interleave

    if ( CL ==  9 )         { MR0_VAL = MR0_VAL | 0x00000000; }  //
    if ( CL == 10 )         { MR0_VAL = MR0_VAL | 0x00000004; }  //
    if ( CL == 11 )         { MR0_VAL = MR0_VAL | 0x00000010; }  //
    if ( CL == 12 )         { MR0_VAL = MR0_VAL | 0x00000014; }  //
    if ( CL == 13 )         { MR0_VAL = MR0_VAL | 0x00000020; }  //
    if ( CL == 14 )         { MR0_VAL = MR0_VAL | 0x00000024; }  //
    if ( CL == 15 )         { MR0_VAL = MR0_VAL | 0x00000030; }  //
    if ( CL == 16 )         { MR0_VAL = MR0_VAL | 0x00000034; }  //
    if ( CL == 18 )         { MR0_VAL = MR0_VAL | 0x00000040; }  //
    if ( CL == 20 )         { MR0_VAL = MR0_VAL | 0x00000044; }  //
    if ( CL == 22 )         { MR0_VAL = MR0_VAL | 0x00000050; }  //
    if ( CL == 24 )         { MR0_VAL = MR0_VAL | 0x00000054; }  //

    if ( DLL_Reset == 1 )   { MR0_VAL = MR0_VAL | 0x00000100; }  // DLL Reset

    if ( WR == 10 )         { MR0_VAL = MR0_VAL | 0x00000000; }  //
    if ( WR == 12 )         { MR0_VAL = MR0_VAL | 0x00000200; }  //
    if ( WR == 14 )         { MR0_VAL = MR0_VAL | 0x00000400; }  //
    if ( WR == 16 )         { MR0_VAL = MR0_VAL | 0x00000600; }  //
    if ( WR == 18 )         { MR0_VAL = MR0_VAL | 0x00000800; }  //
    if ( WR == 20 )         { MR0_VAL = MR0_VAL | 0x00000A00; }  //
    if ( WR == 24 )         { MR0_VAL = MR0_VAL | 0x00000C00; }  //

    if ( DLL_Enable == 1 )  { MR1_VAL = MR1_VAL | 0x00000001; }  // DLL Enable

    if ( ODIC == 0 )        { MR1_VAL = MR1_VAL | 0x00000000; }  // RZQ/7
    if ( ODIC == 1 )        { MR1_VAL = MR1_VAL | 0x00000002; }  // RZQ/5

    if ( AL == 0 )          { MR1_VAL = MR1_VAL | 0x00000000; }  // Disable
    if ( AL == 1 )          { MR1_VAL = MR1_VAL | 0x00000008; }  // CL-1
    if ( AL == 2 )          { MR1_VAL = MR1_VAL | 0x00000010; }  // CL-2

    if ( RTT_NOM == 0 )     { MR1_VAL = MR1_VAL | 0x00000000; }  // RTT_NOM Disable
    if ( RTT_NOM == 1 )     { MR1_VAL = MR1_VAL | 0x00000100; }  // RZQ/4
    if ( RTT_NOM == 2 )     { MR1_VAL = MR1_VAL | 0x00000200; }  // RZQ/2
    if ( RTT_NOM == 3 )     { MR1_VAL = MR1_VAL | 0x00000300; }  // RZQ/6
    if ( RTT_NOM == 4 )     { MR1_VAL = MR1_VAL | 0x00000400; }  // RZQ/1
    if ( RTT_NOM == 5 )     { MR1_VAL = MR1_VAL | 0x00000500; }  // RZQ/5
    if ( RTT_NOM == 6 )     { MR1_VAL = MR1_VAL | 0x00000600; }  // RZQ/3
    if ( RTT_NOM == 7 )     { MR1_VAL = MR1_VAL | 0x00000700; }  // RZQ/7

    if ( TDQS_enable == 1 ) { MR1_VAL = MR1_VAL | 0x00000800; }  // Enable

    if ( CWL == 9  )        { MR2_VAL = MR2_VAL | 0x00000000; }  //
    if ( CWL == 10 )        { MR2_VAL = MR2_VAL | 0x00000008; }  //
    if ( CWL == 11 )        { MR2_VAL = MR2_VAL | 0x00000010; }  //
    if ( CWL == 12 )        { MR2_VAL = MR2_VAL | 0x00000018; }  //
    if ( CWL == 14 )        { MR2_VAL = MR2_VAL | 0x00000020; }  //
    if ( CWL == 16 )        { MR2_VAL = MR2_VAL | 0x00000028; }  //
    if ( CWL == 18 )        { MR2_VAL = MR2_VAL | 0x00000030; }  //

    if ( RTT_WR == 0 )      { MR2_VAL = MR2_VAL | 0x00000000; }  // 
    if ( RTT_WR == 1 )      { MR2_VAL = MR2_VAL | 0x00000200; }  // 
    if ( RTT_WR == 2 )      { MR2_VAL = MR2_VAL | 0x00000400; }  // 
    if ( RTT_WR == 3 )      { MR2_VAL = MR2_VAL | 0x00000600; }  // 

    if ( Write_CRC == 1 )   { MR2_VAL = MR2_VAL | 0x00001000; }  // Enable

    if ( LP_ASR == 0 )      { MR2_VAL = MR2_VAL | 0x00000000; }  // Manual_Normal
    if ( LP_ASR == 1 )      { MR2_VAL = MR2_VAL | 0x00000040; }  // Manual_Reduced
    if ( LP_ASR == 2 )      { MR2_VAL = MR2_VAL | 0x00000080; }  // Manual_Extended
    if ( LP_ASR == 3 )      { MR2_VAL = MR2_VAL | 0x000000C0; }  // ASR

    if ( Geardown == 1 )    { MR3_VAL = MR3_VAL | 0x00000008; }  // Enable

    if ( PDA == 1 )         { MR3_VAL = MR3_VAL | 0x00000010; }  // Enable

    if ( WCL == 4 )         { MR3_VAL = MR3_VAL | 0x00000000; }  //
    if ( WCL == 5 )         { MR3_VAL = MR3_VAL | 0x00000200; }  //
    if ( WCL == 6 )         { MR3_VAL = MR3_VAL | 0x00000400; }  //

    if ( CAL == 0 )         { MR4_VAL = MR4_VAL | 0x00000000; }  // Disable
    if ( CAL == 3 )         { MR4_VAL = MR4_VAL | 0x00000040; }  //
    if ( CAL == 4 )         { MR4_VAL = MR4_VAL | 0x00000080; }  //
    if ( CAL == 5 )         { MR4_VAL = MR4_VAL | 0x000000C0; }  //
    if ( CAL == 6 )         { MR4_VAL = MR4_VAL | 0x00000100; }  //
    if ( CAL == 8 )         { MR4_VAL = MR4_VAL | 0x00000140; }  //

    if ( SREF_Abort == 1 )  { MR4_VAL = MR4_VAL | 0x00000200; }  // Enable

    if ( Read_PRE == 0 )    { MR4_VAL = MR4_VAL | 0x00000000; }  // 1 nCK
    if ( Read_PRE == 1 )    { MR4_VAL = MR4_VAL | 0x00000800; }  // 2 nCK
    if ( Write_PRE == 0 )   { MR4_VAL = MR4_VAL | 0x00000000; }  // 1 nCK
    if ( Write_PRE == 1 )   { MR4_VAL = MR4_VAL | 0x00001000; }  // 2 nCK

    if ( PL == 0 )          { MR5_VAL = MR5_VAL | 0x00000000; }  // Disable
    if ( PL == 4 )          { MR5_VAL = MR5_VAL | 0x00000001; }  //
    if ( PL == 5 )          { MR5_VAL = MR5_VAL | 0x00000002; }  // 
    if ( PL == 6 )          { MR5_VAL = MR5_VAL | 0x00000003; }  // 
    if ( PL == 8 )          { MR5_VAL = MR5_VAL | 0x00000004; }  //

    if ( RTT_PARK == 0 )    { MR5_VAL = MR5_VAL | 0x00000000; }  // RTT_PARK Disable
    if ( RTT_PARK == 1 )    { MR5_VAL = MR5_VAL | 0x00000040; }  // RZQ/4
    if ( RTT_PARK == 2 )    { MR5_VAL = MR5_VAL | 0x00000080; }  // RZQ/2
    if ( RTT_PARK == 3 )    { MR5_VAL = MR5_VAL | 0x000000C0; }  // RZQ/6
    if ( RTT_PARK == 4 )    { MR5_VAL = MR5_VAL | 0x00000100; }  // RZQ/1
    if ( RTT_PARK == 5 )    { MR5_VAL = MR5_VAL | 0x00000140; }  // RZQ/5
    if ( RTT_PARK == 6 )    { MR5_VAL = MR5_VAL | 0x00000180; }  // RZQ/3
    if ( RTT_PARK == 7 )    { MR5_VAL = MR5_VAL | 0x000001C0; }  // RZQ/7

    if ( Data_Mask == 1 )   { MR5_VAL = MR5_VAL | 0x00000400; }  // Enable
    if ( Write_DBI == 1 )   { MR5_VAL = MR5_VAL | 0x00000800; }  // Enable
    if ( Read_DBI  == 1 )   { MR5_VAL = MR5_VAL | 0x00001000; }  // Enable


    if ( tCCD_L == 4  )     { MR6_VAL0 = MR6_VAL0 | 0x00000000; }  //
    if ( tCCD_L == 5  )     { MR6_VAL0 = MR6_VAL0 | 0x00000400; }  // 1600,1866
    if ( tCCD_L == 6  )     { MR6_VAL0 = MR6_VAL0 | 0x00000800; }  // 2133,2400
    if ( tCCD_L == 7  )     { MR6_VAL0 = MR6_VAL0 | 0x00000C00; }  //
    if ( tCCD_L == 8  )     { MR6_VAL0 = MR6_VAL0 | 0x00001000; }  //

    if ( VrefDQ_T_R == 0 )  { MR6_VAL0 = MR6_VAL0 | 0x00000000; }  // VrefDQ Training Range1 
    if ( VrefDQ_T_R == 1 )  { MR6_VAL0 = MR6_VAL0 | 0x00000040; }  // VrefDQ Training Range2 

    MR6_VAL1 = MR6_VAL0 | 0x00000080;                              // VrefDQ Training Entry

    MR6_VAL2 = MR6_VAL0 | 0x00000080;                              // VrefDQ Training Entry
    MR6_VAL2 = MR6_VAL2 | VrefDQ_T_V;                              // VrefDQ Training NewValue set

    MR6_VAL3 = MR6_VAL0 | VrefDQ_T_V;                              // VrefDQ Training Exit

    // change to use DESL, which raises CKE
    Idle(tXPR-1);

    // MRS3
    ModeRegister(MR3,MR3_VAL);
    Idle(tMRD-1);

    // MRS6
    ModeRegister(MR6,MR6_VAL1);           // VrefDQ Training Entry
    Idle(tMRD-1);

    // MRS6
    ModeRegister(MR6,MR6_VAL2);           // VrefDQ Training NewValue set
    Idle(tMRD-1);

    // MRS6
    ModeRegister(MR6,MR6_VAL3);           // VrefDQ Training Exit
    Idle(tMRD-1);

    // MRS5
    ModeRegister(MR5,MR5_VAL);
    Idle(tMRD-1);

    // MRS4
    ModeRegister(MR4,MR4_VAL);
    Idle(tMRD-1);

    // MRS2
    ModeRegister(MR2,MR2_VAL);
    Idle(tMRD-1);

    // MRS1
    ModeRegister(MR1,MR1_VAL);
    Idle(tMRD-1);

    // MRS0
    ModeRegister(MR0,MR0_VAL);
    Idle(tMOD-1);

    // issue ZQCcal command...
    ZQCalibration();
    Idle(tZQinit-1);

    pad(PreISPad){}

    REFRESH_CTRL(START);
}

     
