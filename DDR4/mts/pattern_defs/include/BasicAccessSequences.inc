//
// defining pattern common memory access functionality
//
#include "../../device_defs/include/DDR4Operations.inc"

// write background 
WriteBackground(bank,dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8)
{
   for (row=xStart; row<xStop; row++)
   {
      //refresh insertion point(if needed)
      //refresh has to be tester cycle aligned; pad() quarantees alignment
      // padding only done in case refresh enabled and not tester cycle aligned
      pad(PreISPad){}  
      is{}  
  
      Activate(row, bank);
      Idle(tRCD-1);
      
      // WRITE burst with write preamble
      Write_Preamb();
      for (col=yStart; col<yStop; col+=8)
      {
         Write(row,col,bank,UNMASK,dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8);
      }
      Idle(tWR+CWL+PL-1);
  
      Precharge(bank);
      Idle(tRP-1);
   }
}

// read background 
ReadBackground(bank,dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8)
{
   for (row=xStart; row<xStop; row++)
   {
      //refresh insertion point(if needed)
      //refresh has to be tester cycle aligned; pad() quarantees alignment
      //padding only done in case refresh enabled and not tester cycle aligned
      pad(PreISPad){}  
      is{}  
     
      Activate(row, bank);
      Idle(tRCD-1);
      
      // READ burst
      for (col=yStart; col<yStop; col+=8)
      {
         Read(row,col,bank,dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8);
      }
      Idle(tRTP+PL-1);
     
      Precharge(bank);
      Idle(tRP-1);
   }
}

// read background with expecting 2/8 in one burst
ReadBackground_XXEXXXEX(bank,dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8)
{
   for (row=xStart; row<xStop; row++)
   {
      //refresh insertion point(if needed)
      //refresh has to be tester cycle aligned; pad() quarantees alignment
      //padding only done in case refresh enabled and not tester cycle aligned
      pad(PreISPad){}  
      is{}  
     
      Activate(row, bank);
      Idle(tRCD-1);
      
      // READ burst
      for (col=yStart; col<yStop; col+=8)
      {
         Read_XXEXXXEX(row,col,bank,dw1,dw2,dw3,dw4,dw5,dw6,dw7,dw8);
      }
      Idle(tRTP+PL-1);
     
      Precharge(bank);
      Idle(tRP-1);
   }
}


/////////////////////////////////////////
// PADDING Routines
// 
// Deselect operation: 1 tCK resolution
////////////////////////////////////////

EndProgPad()
{
  Deselect();
}

PreISPad()
{
  Deselect();
}
