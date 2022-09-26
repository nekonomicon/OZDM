#include "extdll.h"
#include "util.h"
#include <windows.h>

// the ini file char
char m_iniFile[MAX_PATH];

// goes in game.cpp above the game dll with the cvars
//unsigned long mSetupIniPath (void);

// goes in game.cpp at the end of the cvar register section
//	mSetupIniPath();

DWORD mSetupIniPath (void)
{
    char file[MAX_PATH];
    char name[MAX_PATH];
    char dir[MAX_PATH];
	char text[1024];

    sprintf (file, "oz/oz.ini");

    // get the current directory
    GetCurrentDirectory(MAX_PATH, dir);

    // form the path string
    sprintf(name, "%s\\%s", dir, file);

    // set up the string, and print it to the console
    strcpy (m_iniFile, name);
    // gi.dprintf("Initialization file: %s\n", m_iniFile);
    sprintf (text, "Initialization file: %s\n", m_iniFile);

    // successful
    return (ERROR_SUCCESS);
}

/*

#include <windows.h>// ini
extern char m_iniFile[MAX_PATH];// ini

GetPrivateProfileInt("section", "key", 0, m_iniFile)

kRenderFxGlowShell
pPlayer->pev->renderfx |= kRenderFxGlowShell;
pev->rendercolor = Vector(255, 255, 255);		
pev->renderamt = 3;	

*/