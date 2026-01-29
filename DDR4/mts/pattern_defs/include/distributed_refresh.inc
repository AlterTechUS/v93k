//
// refresh routine used for static refresh implementation
// static  --> refresh routine compiled into pattern
// dynamic --> refresh routine added on the fly by the test processor
//
distributed_refresh_routine() 
{
   // Issue a Refresh command.
   Refresh();
   Idle(tRFC-1);
   
   pad(EndProgPad){}
}
