#pragma once

#define LOCALE_ENG 0
#define LOCALE_RU  1

const char* LOCALE_PREMENU[2]       = { "Load into world to see more options", reinterpret_cast<const char*>(u8"Загрузите мир чтобы увидеть больше функций") };
const char* LOCALE_MENU[2]          = { "Menu", reinterpret_cast<const char*>(u8"Меню") };
const char* LOCALE_NONETWORK[2]     = { "Mod prevents you from playing multiplayer!", reinterpret_cast<const char*>(u8"Мод не позволит вам играть в мультиплеер!") };
const char* LOCALE_BOOSTY[2]        = { 
    "If you want to support my work or get access to more options:\n"
    " - Be able to play multiplayer\n"
    " - Fast tree cutting\n"
    " - Unlock achievements in steam\n"
    " - Get all story items\n"
    " - Locations ESP (Yacht, pit, plane, caves)\n"
    "You can buy a subscription on my boosty <link>\n", 
    reinterpret_cast<const char*>(u8"Если вы хотите поддержать мою работу\n или получить доступ к другим функциям:\n"
    " - Возможность играть в мультиплеере\n"
    " - Быстрая рубка деревьев\n"
    " - Открыть стимовские достижения\n"
    " - Получить все сюжетные предметы\n"
    " - Показывать локации (яхта, яма, самолет, пещеры)\n"
    "Вы можете купить подписку на boosty <ссылка>\n")
};

const char* LOCALE_GODMODE[2]       = { "Godmode", reinterpret_cast<const char*>(u8"Бессмертие") };
const char* LOCALE_VITALS[2]        = { "VITALS", reinterpret_cast<const char*>(u8"СТАТЫ") };
const char* LOCALE_MISC[2]          = { "MISC", reinterpret_cast<const char*>(u8"РАЗНОЕ") };
const char* LOCALE_MOVEMENT[2]      = { "MOVEMENT", reinterpret_cast<const char*>(u8"ПЕРЕДВИЖЕНИЕ") };
const char* LOCALE_INFENERGY[2]     = { "Infinite stamina", reinterpret_cast<const char*>(u8"Бесконечная выносливость") };
const char* LOCALE_INFBATTERY[2]    = { "Infinite battery", reinterpret_cast<const char*>(u8"Бесконечный заряд батареи") };
const char* LOCALE_FULLNESS[2]      = { "Max fullness", reinterpret_cast<const char*>(u8"Полная сытость") };
const char* LOCALE_THIRST[2]        = { "No thirst", reinterpret_cast<const char*>(u8"Нет жажды") };
const char* LOCALE_BUILDING[2]      = { "Free crafting", reinterpret_cast<const char*>(u8"Крафт без ресурсов") };
const char* LOCALE_UNLIMITEMS[2]    = { "Unlimited items", reinterpret_cast<const char*>(u8"Бесконечные предметы") };
const char* LOCALE_JUMPAMPL[2]      = { "Jump amplification", reinterpret_cast<const char*>(u8"Усиление прыжка") };
const char* LOCALE_FALLDMG[2]       = { "No fall damage", reinterpret_cast<const char*>(u8"Нет урона от падения") };
const char* LOCALE_MOVACCEL[2]      = { "Movement acceleration", reinterpret_cast<const char*>(u8"Усорение передвижения") };
const char* LOCALE_COLD[2]          = { "Always warm", reinterpret_cast<const char*>(u8"Всегда тепло") };
const char* LOCALE_SANITY[2]        = { "Max sanity", reinterpret_cast<const char*>(u8"Нормальная психика") };
const char* LOCALE_RANDOUTFIT[2]    = { "Random outfit", reinterpret_cast<const char*>(u8"Случайная одежда") };
const char* LOCALE_STORYITEMS[2]    = { "Get story items", reinterpret_cast<const char*>(u8"Получить сюжетные предметы") };
const char* LOCALE_ALLITEMS[2]      = { "Get non-story items", reinterpret_cast<const char*>(u8"Получить все предметы") };
const char* LOCALE_INFFUEL[2]       = { "Infinite chainsaw fuel", reinterpret_cast<const char*>(u8"Бесконечное топливо у бензопилы") };
const char* LOCALE_HAIRSPRAY[2]     = { "Infinite hairspray", reinterpret_cast<const char*>(u8"Бесконечный лак для волос") };
const char* LOCALE_OHK[2]           = { "One hit kills", reinterpret_cast<const char*>(u8"Мгновенные убийства") };
const char* LOCALE_DOLIGHT[2]       = { "Illuminate around", reinterpret_cast<const char*>(u8"Освещать вокруг") };
const char* LOCALE_ACHIEVS[2]       = { "Unlock all achievements", reinterpret_cast<const char*>(u8"Открыть все достижения") };
const char* LOCALE_TREECUT[2]       = { "Fast tree cut", reinterpret_cast<const char*>(u8"Быстрая рубка деревьев") };
const char* LOCALE_NONE[2]          = { "None", reinterpret_cast<const char*>(u8"Убрать") };
const char* LOCALE_ANIMESP[2]       = { "Animals ESP", reinterpret_cast<const char*>(u8"Показать животных") };
const char* LOCALE_CANNESP[2]       = { "Enemies ESP", reinterpret_cast<const char*>(u8"Показать врагов") };
const char* LOCALE_LOCATIONSESP[2]  = { "Locations ESP", reinterpret_cast<const char*>(u8"Показать особые места") };
const char* LOCALE_SPEEDCLOCK[2]    = { "Speed up clock time", reinterpret_cast<const char*>(u8"Ускорить игровое время") };
const char* LOCALE_ARMOR[2]         = { "Armor", reinterpret_cast<const char*>(u8"Броня") };
const char* LOCALE_ITEM[2]          = { "Item", reinterpret_cast<const char*>(u8"Предмет") };
const char* LOCALE_RAIN[2]          = { "Rain", reinterpret_cast<const char*>(u8"Дождь") };
const char* LOCALE_CREATURES[2]     = { "CREATURES", reinterpret_cast<const char*>(u8"СУЩЕСТВА") };
const char* LOCALE_ENEMY[2]         = { "Enemy", reinterpret_cast<const char*>(u8"Враг") };
const char* LOCALE_ANIMAL[2]        = { "Animal", reinterpret_cast<const char*>(u8"Животное") };
const char* LOCALE_INVISIBLE[2]     = { "Enemies cant see you", reinterpret_cast<const char*>(u8"Враги вас не видят") };
const char* LOCALE_INFCONSUME[2]    = { "Infinite consumables", reinterpret_cast<const char*>(u8"Расходные предметы не кончаются") };
const char* LOCALE_SKIPPLANE[2]     = { "Skip plane crash", reinterpret_cast<const char*>(u8"Пропускать крушение самолета") };
const char* LOCALE_GENERAL[2]       = { "GENERAL", reinterpret_cast<const char*>(u8"ОБЩЕЕ") };

// Buttons
const char* LOCALE_GET_ARMOR[2]     = { "Get##Armor", reinterpret_cast<const char*>(u8"Получить##Armor") };
const char* LOCALE_GET_ITEM[2]      = { "Get##Item", reinterpret_cast<const char*>(u8"Получить##Item") };
const char* LOCALE_SET[2]           = { "Set##Rain", reinterpret_cast<const char*>(u8"Установить##Rain") };
const char* LOCALE_SPAWN_ENEMY[2]   = { "Spawn##Enemy", reinterpret_cast<const char*>(u8"Заспавнить##Enemy") };
const char* LOCALE_SPAWN_ANIMAL[2]  = { "Spawn##Animal", reinterpret_cast<const char*>(u8"Заспавнить##Animal") };

// Menu bars
const char* LOCALE_PLAYER[2]        = { "Player", reinterpret_cast<const char*>(u8"Игрок") };
const char* LOCALE_ITEMS[2]         = { "Items", reinterpret_cast<const char*>(u8"Предметы") };
const char* LOCALE_WORLD[2]         = { "World", reinterpret_cast<const char*>(u8"Мир") };
const char* LOCALE_SETTINGS[2]      = { "Settings", reinterpret_cast<const char*>(u8"Настройки") };
const char* LOCALE_LANGUAGE[2]      = { "Language", reinterpret_cast<const char*>(u8"Язык") };

// Animals
const char* LOCALE_BAT[2]           = { "Bat", reinterpret_cast<const char*>(u8"Варан") };
const char* LOCALE_BIRD[2]          = { "Bird", reinterpret_cast<const char*>(u8"Птица") };
const char* LOCALE_GOOSE[2]         = { "Goose", reinterpret_cast<const char*>(u8"Гусь") };
const char* LOCALE_DEER[2]          = { "Deer", reinterpret_cast<const char*>(u8"Олень") };
const char* LOCALE_FISH[2]          = { "Fish", reinterpret_cast<const char*>(u8"Рыба") };
const char* LOCALE_OYSTER[2]        = { "Oyster", reinterpret_cast<const char*>(u8"Устрица") };
const char* LOCALE_RABBIT[2]        = { "Rabbit", reinterpret_cast<const char*>(u8"Кролик") };
const char* LOCALE_LIZARD[2]        = { "Lizard", reinterpret_cast<const char*>(u8"Варан") };
const char* LOCALE_RACCOON[2]       = { "Raccoon", reinterpret_cast<const char*>(u8"Енот") };
const char* LOCALE_SEAGULL[2]       = { "Seagull", reinterpret_cast<const char*>(u8"Чайка") };
const char* LOCALE_TORTOISE[2]      = { "Tortoise", reinterpret_cast<const char*>(u8"Черепаха") };
const char* LOCALE_TURTLE[2]        = { "Turtle", reinterpret_cast<const char*>(u8"Черепаха") };
const char* LOCALE_SQUIRREL[2]      = { "Squirrel", reinterpret_cast<const char*>(u8"Белка") };
const char* LOCALE_BOAR[2]          = { "Boar", reinterpret_cast<const char*>(u8"Кабан") };
const char* LOCALE_CROCODILE[2]     = { "Crocodile", reinterpret_cast<const char*>(u8"Крокодил") };
const char* LOCALE_SHARK[2]         = { "Shark", reinterpret_cast<const char*>(u8"Акула") };

// Enemies
const char* LOCALE_CANNIBAL[2]     =  { "Cannibal", reinterpret_cast<const char*>(u8"Каннибал") };
const char* LOCALE_ARMSY[2]        =  { "Armsy", reinterpret_cast<const char*>(u8"Рукастый") };
const char* LOCALE_VIRGINIA[2]     =  { "Virginia", reinterpret_cast<const char*>(u8"Вирджиния") };
const char* LOCALE_COWMAN[2]       =  { "Cowman", reinterpret_cast<const char*>(u8"Толстяк") };
const char* LOCALE_GIRL[2]         =  { "Girl", reinterpret_cast<const char*>(u8"Девочка") };

// Locations
const char* LOCALE_YACHT[2]         = { "Yacht", reinterpret_cast<const char*>(u8"Яхта") };
const char* LOCALE_PLANE[2]         = { "Plane", reinterpret_cast<const char*>(u8"Самолет") };
const char* LOCALE_PIT[2]           = { "Pit", reinterpret_cast<const char*>(u8"Яма") };
const char* LOCALE_CAVE[2]          = { "Cave", reinterpret_cast<const char*>(u8"Пещера") };

// Combos
const char* LOCALE_ENEMIES[2][13] = {
    {
        "Skinny male", 
        "Skinny female", 
        "Skinny pale", 
        "Male", 
        "Female", 
        "Fireman", 
        "Dynamiteman", 
        "Pale", 
        "Armsy", 
        "Virginia", 
        "Baby", 
        "Cowman", 
        "Girl"
    },
    {
        reinterpret_cast<const char*>(u8"Тощий мужчина"),
        reinterpret_cast<const char*>(u8"Тощая женщина"),
        reinterpret_cast<const char*>(u8"Тощий бледный"),
        reinterpret_cast<const char*>(u8"Обычный мужчина"),
        reinterpret_cast<const char*>(u8"Обычная женщина"),
        reinterpret_cast<const char*>(u8"Пожарный"),
        reinterpret_cast<const char*>(u8"Динамитчик"),
        reinterpret_cast<const char*>(u8"Обычный бледный"),
        reinterpret_cast<const char*>(u8"Рукастый"),
        reinterpret_cast<const char*>(u8"Вирджиния"),
        reinterpret_cast<const char*>(u8"Младенец"),
        reinterpret_cast<const char*>(u8"Толстяк"),
        reinterpret_cast<const char*>(u8"Девочка")
    }
};

const char* LOCALE_ANIMALS[2][8] = {
    {
        "Rabbit",
        "Lizard",
        "Deer",
        "Tortoise",
        "Raccoon",
        "Squirrel",
        "Boar",
        "Crocodile"
    },
    {
        reinterpret_cast<const char*>(u8"Кролик"),
        reinterpret_cast<const char*>(u8"Варан"),
        reinterpret_cast<const char*>(u8"Олень"),
        reinterpret_cast<const char*>(u8"Черепаха"),
        reinterpret_cast<const char*>(u8"Енот"),
        reinterpret_cast<const char*>(u8"Белка"),
        reinterpret_cast<const char*>(u8"Кабан"),
        reinterpret_cast<const char*>(u8"Крокодил"),
    }
};

const char* LOCALE_ARMORS[2][6] = {
    {
        "Lizard",
        "Deer",
        "Stealth",
        "Bone",
        "Creepy",
        "Warmsuit"
    },
    {
        reinterpret_cast<const char*>(u8"Ящера"),
        reinterpret_cast<const char*>(u8"Оленья"),
        reinterpret_cast<const char*>(u8"Камуфляж"),
        reinterpret_cast<const char*>(u8"Костяная"),
        reinterpret_cast<const char*>(u8"Жуткая"),
        reinterpret_cast<const char*>(u8"Теплый костюм"),
    }
};

const char* LOCALE_RAIN_TYPES[2][4] = {
    {
        "Off",
        "Light",
        "Medium",
        "Heavy"
    },
    {
        reinterpret_cast<const char*>(u8"Выключить"),
        reinterpret_cast<const char*>(u8"Слабый"),
        reinterpret_cast<const char*>(u8"Средний"),
        reinterpret_cast<const char*>(u8"Сильный"),
    }
};

// Tips
const char* LOCALE_TIP_FALL[2] = { "Removes damage and animation after fall", reinterpret_cast<const char*>(u8"Убирает урон и анимацию после падения") };
const char* LOCALE_TIP_TREECUT[2] = { "Instantly cut undamaged trees down", reinterpret_cast<const char*>(u8"Моментально срубает неповрежденные деревья") };
const char* LOCALE_TIP_CONSUMABLES[2] = { "Infinite molotovs, bombs, arrows, etc.", reinterpret_cast<const char*>(u8"Бесконечные молотовы, бомбы, стрелы и тп.") };
const char* LOCALE_TIP_ENEMIES[2] = { "Takes some time. DO NOT SPAM", reinterpret_cast<const char*>(u8"Занимает какое-то время. НЕ СПАМЬТЕ!") };
const char* LOCALE_TIP_PLANE[2] = { "You will spawn at the beach", reinterpret_cast<const char*>(u8"Вы появитесь на пляже") };
const char* LOCALE_TIP_ESP[2] = { "Shows yacht, pit, plane, caves and distance to them", reinterpret_cast<const char*>(u8"Показывает яхту, яму, самолет и пещеры,\nа также расстояние до них") };