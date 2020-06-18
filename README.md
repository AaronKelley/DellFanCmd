DellFanCmd can be used to tell the embedded controller to let go of fan control.  After this is done, a fan control tool like SpeedFan or Hwinfo64 can be used to manage the fans, with custom behavior based on temperature thresholds set up to your liking.  For information on how to use the tool, see this thread on NotebookReview:
http://forum.notebookreview.com/threads/dellfancmd-%E2%80%93-tool-to-enable-manual-custom-control-of-the-laptop-fans.827106/

This project has been superseded by DellFanManagement, which contains a C# implementation of DellFanCmd.  There will be no further updates to C++ DellFanCmd from me.
https://github.com/AaronKelley/DellFanManagement
