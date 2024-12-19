#ifdef HAXSDK_STATIC_FIELD
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", Scene, ActiveMB, void*);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", Scene, Yacht, Unity::GameObject*);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", Scene, MutantControler, mutantController*);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", Scene, WeatherSystem, WeatherSystem*);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "", Cheats, GodMode, bool);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "", Cheats, InfiniteEnergy, bool);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "", Cheats, Creative, bool);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "", Cheats, UnlimitedHairspray, bool);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", LocalPlayer, GameObject, Unity::GameObject*);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", LocalPlayer, Transform, Unity::Transform*);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Utils", LocalPlayer, Stats, PlayerStats*);
HAXSDK_STATIC_FIELD("Assembly-CSharp", "TheForest.Player", Achievements, Data, System::Array<AchievementData*>*);
HAXSDK_STATIC_FIELD("Assembly-CSharp-firstpass", "PathologicalGames", PoolManager, Pools, SpawnPoolsDict*);
#undef HAXSDK_STATIC_FIELD
#endif

#ifdef HAXSDK_FIELD_OFFSET
HAXSDK_FIELD_OFFSET("Assembly-CSharp", "", mutantController, activeCannibals);
HAXSDK_FIELD_OFFSET("Assembly-CSharp", "", PlayerStats, Fullness);
HAXSDK_FIELD_OFFSET("Assembly-CSharp", "", PlayerStats, Thirst);
HAXSDK_FIELD_OFFSET("Assembly-CSharp", "", PlayerStats, BatteryCharge);
HAXSDK_FIELD_OFFSET("Assembly-CSharp", "", FirstPersonCharacter, allowFallDamage);
HAXSDK_FIELD_OFFSET("Assembly-CSharp", "", FirstPersonCharacter, speed);
HAXSDK_FIELD_OFFSET("Assembly-CSharp", "TheForest.Items.Inventory", InventoryItem, _amount);
HAXSDK_FIELD_OFFSET("Assembly-CSharp-firstpass", "PathologicalGames", SpawnPool, _spawned);
#undef HAXSDK_FIELD_OFFSET
#endif

#ifdef HAXSDK_FUNCTION
HAXSDK_FUNCTION("Assembly-CSharp", "", PlayerStats, Update, "System.Void()");
HAXSDK_FUNCTION("Assembly-CSharp", "", PlayerStats, get_Warm, "System.Boolean()");
HAXSDK_FUNCTION("Assembly-CSharp", "", FirstPersonCharacter, HandleLanded, "System.Void()");
HAXSDK_FUNCTION("Assembly-CSharp", "", FirstPersonCharacter, HandleWalkingSpeedOptions, "System.Void()");
HAXSDK_FUNCTION("Assembly-CSharp", "", FirstPersonCharacter, HandleRunningStaminaAndSpeed, "System.Void()");
HAXSDK_FUNCTION("Assembly-CSharp", "", FirstPersonCharacter, CalculateJumpVerticalSpeed, "System.Single()");
HAXSDK_FUNCTION("Assembly-CSharp", "", EnemyHealth, Hit, "System.Void(System.Int32)");
HAXSDK_FUNCTION("Assembly-CSharp", "", EnemyHealth, Die, "System.Void()");
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest", DebugConsole, _addClothingOutfitRandom, "System.Void(System.String)");
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest", DebugConsole, _addAllStoryItems, "System.Void(System.Object)");
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest", DebugConsole, _addAllItems, "System.Void(System.Object)");
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest.Utils", LocalPlayer, Awake, "System.Void()");
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest.Utils", LocalPlayer, OnDestroy, "System.Void()");
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest.Player", AccountInfo, UnlockAchievement, "System.Boolean(TheForest.Player.AchievementData)");
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest.World", WeatherSystem, TurnOn, "System.Void(TheForest.World.WeatherSystem.RainTypes)");
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest.Items.World", BatteryBasedLight, Update, "System.Void()");
HAXSDK_FUNCTION("Assembly-CSharp", "TheForest.Items.Inventory", InventoryItem, Add, "System.Int32(System.Int32,System.Boolean)");
HAXSDK_FUNCTION("Assembly-CSharp-firstpass", "PathologicalGames", SpawnPoolsDict, get_Item, "PathologicalGames.SpawnPool(System.String)");
#undef HAXSDK_FUNCTION_EXT
#undef HAXSDK_FUNCTION
#endif