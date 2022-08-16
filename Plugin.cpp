#include "Plugin.h"
#include <loader.hpp>
#include <hexrays.hpp>
#include "AllocaFixer.hpp"
#include "MicrocodeExplorer.hpp"
#include "PatternDeobfuscate.hpp"
#include "Unflattener.hpp"

#define USE_DANGEROUS_FUNCTIONS 

ObfCompilerOptimizer hook;
CFUnflattener cfu;

// Hex-Rays API pointer
hexdsp_t* hexdsp = NULL;

bool idaapi Plugin::run(size_t arg)
{

	FixCallsToAllocaProbe();
	return true;
	

	ShowMicrocodeExplorer();
	return true;
	
}

Plugin::Plugin()
{

}

Plugin::~Plugin()
{
	if (hexdsp != NULL)
	{

		// Uninstall our block and instruction optimization classes.
#if DO_OPTIMIZATION
		remove_optinsn_handler(&hook);
		remove_optblock_handler(&cfu);

		// I couldn't figure out why, but my plugin would segfault if it tried
		// to free mop_t pointers that it had allocated. Maybe hexdsp had been
		// set to NULL at that point, so the calls to delete crashed? Anyway,
		// cleaning up before we unload solved the issues.
		cfu.Clear(true);
#endif
		term_hexrays_plugin();
	}
}

//--------------------------------------------------------------------------
static plugmod_t* idaapi init()
{
	if (!init_hexrays_plugin())
		return 0; // no decompiler
	const char* hxver = get_hexrays_version();
	msg("Hex-rays version %s has been detected, %s ready to use\n", hxver, PLUGIN.wanted_name);

	// Install our block and instruction optimization classes.
#if DO_OPTIMIZATION
	install_optinsn_handler(&hook);
	install_optblock_handler(&cfu);
#endif
	return new Plugin();
}


//--------------------------------------------------------------------------
static const char comment[] = "Show microcode";


//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN =
{
	IDP_INTERFACE_VERSION,
	PLUGIN_MULTI,         // plugin flags
	init,                 // initialize
	NULL,                 // terminate. this pointer may be NULL.
	NULL,                  // invoke plugin
	comment,              // long comment about the plugin
	"",                   // multiline help about the plugin
	"Microcode explorer", // the preferred short name of the plugin
	""                    // the preferred hotkey to run the plugin
};
