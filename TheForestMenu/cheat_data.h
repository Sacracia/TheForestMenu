#ifdef HAXSDK_STATIC_FIELD
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", Scene, ActiveMB, void*);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", Scene, Yacht, GameObject*);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", Scene, MutantControler, mutantController*);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "", Cheats, GodMode, bool);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "", Cheats, InfiniteEnergy, bool);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "", Cheats, Creative, bool);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "", Cheats, UnlimitedHairspray, bool);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", LocalPlayer, GameObject, GameObject*);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", LocalPlayer, Transform, Transform*);
#undef HAXSDK_STATIC_FIELD
#endif

#ifdef HAXSDK_FIELD_OFFSET
HAXSDK_FIELD_OFFSET("Assembly-CSharp", "", mutantController, activeCannibals);

#undef HAXSDK_FIELD_OFFSET
#endif

#ifdef HAXSDK_FUNCTION
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest", DebugConsole, _addClothingOutfitRandom, "System.Void(System.String)");
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest", DebugConsole, _addAllStoryItems, "System.Void(System.Object)");
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest", DebugConsole, _addAllItems, "System.Void(System.Object)");
#undef HAXSDK_FUNCTION_EXT
#undef HAXSDK_FUNCTION
#endif