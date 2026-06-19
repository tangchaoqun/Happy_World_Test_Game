#include "StoryManager.h"
#include "../Player/Player.h"
#include "../Manager/GameDataManager.h"

USING_NS_CC;

StoryManager* StoryManager::s_instance = nullptr;

StoryManager* StoryManager::getInstance() {
    if (!s_instance) {
        s_instance = new StoryManager();
    }
    return s_instance;
}

// ===== 初始化 =====

void StoryManager::initialize() {
    setupDefaultStory();
    loadProgress();
    log("[StoryManager] Initialized - %zu quests, %zu chapters",
        m_questDatabase.size(), m_chapters.size());
}

void StoryManager::setupDefaultStory() {
    // ===== 第一章：初入江湖 =====
    StoryChapter chapter1;
    chapter1.id = 1;
    chapter1.title = "初入江湖";
    chapter1.description = "你醒来在一个陌生的村庄，失去了记忆...";

    // 序章对话 - 醒来
    chapter1.dialogs.push_back({
        1, "???", "（缓缓睁开眼）\n头好痛...这是哪里？",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter1.dialogs.push_back({
        2, "老村长", "年轻人，你终于醒了。三天前村民在村东废墟发现你昏迷不醒，就把你带回来了。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter1.dialogs.push_back({
        3, "你", "谢谢您的救命之恩。但我什么都想不起来了...我是谁？",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter1.dialogs.push_back({
        4, "老村长", "失忆了？唉，最近村外不太平，魔物横行。许是被魔兽所伤。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter1.dialogs.push_back({
        5, "老村长", "既然无处可去，不如先在村里安顿下来。不过...村子最近遇到些麻烦。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter1.dialogs.push_back({
        6, "老村长", "村外出现了一群史莱姆，袭击了运送草药的村民。你愿意帮忙清理吗？",
        "npc.png", {}, false, nullptr, nullptr, 1001, 0
    });
    chapter1.dialogs.push_back({
        7, "你", "没问题！我这就去！",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[1] = chapter1;

    // ===== 第二章：铁匠的考验 =====
    StoryChapter chapter2;
    chapter2.id = 2;
    chapter2.title = "铁匠的考验";
    chapter2.description = "击退史莱姆后，你需要找到铁匠获取装备...";
    chapter2.requiredChapterId = 1;

    chapter2.dialogs.push_back({
        10, "老村长", "做得好！那些史莱姆一时半会儿不敢靠近了。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter2.dialogs.push_back({
        11, "老村长", "不过以你现在的实力，遇到更强的魔物就危险了。去找铁匠老张，让他给你打造件趁手的兵器。",
        "npc.png", {}, false, nullptr, nullptr, 1002, 0
    });
    chapter2.dialogs.push_back({
        12, "铁匠老张", "村长让你来的？哼，想要装备，得先证明你有这个实力。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter2.dialogs.push_back({
        13, "铁匠老张", "村西矿洞最近被一群魔化蜘蛛占据了，帮我清理5只，我就给你打造武器。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter2.dialogs.push_back({
        14, "你", "交给我吧！",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[2] = chapter2;

    // ===== 第三章：药剂师的请求 =====
    StoryChapter chapter3;
    chapter3.id = 3;
    chapter3.title = "药剂师的请求";
    chapter3.description = "药师需要稀有草药来炼制疗伤药...";
    chapter3.requiredChapterId = 2;

    chapter3.dialogs.push_back({
        20, "铁匠老张", "不错，身手还可以。这把铁剑你拿着，虽然普通，但总比空手强。",
        "npc.png", {}, false, nullptr, nullptr, 0, 1002
    });
    chapter3.dialogs.push_back({
        21, "铁匠老张", "对了，药师小李子也在找你，他说有急事。",
        "npc.png", {}, false, nullptr, nullptr, 1003, 0
    });
    chapter3.dialogs.push_back({
        22, "药师", "谢天谢地你来了！我急需几株龙血草来炼制疗伤药，但龙血草只在沼泽深处生长。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter3.dialogs.push_back({
        23, "药师", "采集3株龙血草给我，我会给你丰厚的报酬。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter3.dialogs.push_back({
        24, "你", "沼泽深处...听起来很危险，但我去试试。",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[3] = chapter3;

    // ===== 第四章：暗影威胁 =====
    StoryChapter chapter4;
    chapter4.id = 4;
    chapter4.title = "暗影威胁";
    chapter4.description = "村外出现了异常强大的魔物...";
    chapter4.requiredChapterId = 3;

    chapter4.dialogs.push_back({
        30, "老村长", "大事不好了！村东废墟出现了一只巨大的暗影狼，已经伤了好几个猎人！",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter4.dialogs.push_back({
        31, "老村长", "勇士，这是生死存亡的时刻。请你击败暗影狼，保护村庄！",
        "npc.png", {}, false, nullptr, nullptr, 1004, 0
    });
    chapter4.dialogs.push_back({
        32, "你", "交给我！我一定保护好村子！",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[4] = chapter4;

    // ===== 第五章：暗影狼的秘密 =====
    StoryChapter chapter5;
    chapter5.id = 5;
    chapter5.title = "暗影狼的秘密";
    chapter5.description = "击败暗影狼后，发现了令人不安的线索...";
    chapter5.requiredChapterId = 4;

    chapter5.dialogs.push_back({
        40, "你", "（喘息）终于...倒下了。",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter5.dialogs.push_back({
        41, "你", "等等，这是什么？暗影狼身上有个奇怪的印记...",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter5.dialogs.push_back({
        42, "老村长", "（查看印记后脸色大变）这是...暗影议会的标记！不可能，他们应该在一百年前就被封印了！",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter5.dialogs.push_back({
        43, "老村长", "年轻人，事情比我们想象的严重得多。暗影狼不是普通的魔物，它是被人召唤出来的！",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter5.dialogs.push_back({
        44, "老村长", "我需要你把这枚印记交给王城的圣殿骑士团。只有他们能解读其中的秘密。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter5.dialogs.push_back({
        45, "老村长", "但在出发之前，先去猎人大厅找王猎户。他说有重要的事情要告诉你。",
        "npc.png", {}, false, nullptr, nullptr, 1005, 0
    });
    chapter5.dialogs.push_back({
        46, "你", "明白了，我这就去。",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[5] = chapter5;

    // ===== 第六章：猎人的委托 =====
    StoryChapter chapter6;
    chapter6.id = 6;
    chapter6.title = "猎人的委托";
    chapter6.description = "猎人大厅的首领需要你的帮助...";
    chapter6.requiredChapterId = 5;

    chapter6.dialogs.push_back({
        50, "王猎户", "你就是那个击败暗影狼的年轻人？果然英雄出少年啊！",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter6.dialogs.push_back({
        51, "王猎户", "最近村北的森林出现了一只巨型野猪王，已经有三个猎人被它重伤了。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter6.dialogs.push_back({
        52, "王猎户", "我需要你去收集一些野猪的獠牙，顺便摸清它的底细。收集8个野猪獠牙给我。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter6.dialogs.push_back({
        53, "你", "野猪王...听起来是个棘手的对手。",
        "player.png", {}, false, nullptr, nullptr, 1006, 0
    });
    chapter6.dialogs.push_back({
        54, "王猎户", "小心点，那家伙力气大得惊人。而且...我总觉得它也有些不对劲。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter6.dialogs.push_back({
        55, "你", "又是暗影议会的痕迹？我会注意的。",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[6] = chapter6;

    // ===== 第七章：远方的使者 =====
    StoryChapter chapter7;
    chapter7.id = 7;
    chapter7.title = "远方的使者";
    chapter7.description = "一位来自王城的神秘使者带来了重要消息...";
    chapter7.requiredChapterId = 6;

    chapter7.dialogs.push_back({
        60, "王猎户", "干得漂亮！这些獠牙品质很好，看来你确实有两下子。",
        "npc.png", {}, false, nullptr, nullptr, 0, 1006
    });
    chapter7.dialogs.push_back({
        61, "神秘使者", "（从阴影中走出）不愧是击败暗影狼的勇士。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter7.dialogs.push_back({
        62, "你", "你是谁？！",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter7.dialogs.push_back({
        63, "神秘使者", "别紧张，我是王城圣殿骑士团的密使。奉大团长之命前来。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter7.dialogs.push_back({
        64, "神秘使者", "暗影议会的封印正在松动。全国各处都报告了类似的魔物暴动。王城需要你的力量。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter7.dialogs.push_back({
        65, "神秘使者", "但在前往王城之前，你必须变得更强。去矿洞深处，那里有一件古老的遗物。",
        "npc.png", {}, false, nullptr, nullptr, 1007, 0
    });
    chapter7.dialogs.push_back({
        66, "神秘使者", "只有获得那件遗物，你才有资格踏入王城。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter7.dialogs.push_back({
        67, "你", "古老遗物...我会找到的。",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[7] = chapter7;

    // ===== 第八章：矿洞深处的秘密 =====
    StoryChapter chapter8;
    chapter8.id = 8;
    chapter8.title = "矿洞深处的秘密";
    chapter8.description = "深入矿洞最深处，寻找古老的遗物...";
    chapter8.requiredChapterId = 7;

    chapter8.dialogs.push_back({
        70, "你", "（进入矿洞最深处）这里和之前完全不同...空气中弥漫着古老的魔力。",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter8.dialogs.push_back({
        71, "你", "那是...一座祭坛？上面放着一块发光的石头。",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter8.dialogs.push_back({
        72, "守护者残魂", "（空灵的声音）是谁...打扰了千年的沉睡...",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter8.dialogs.push_back({
        73, "你", "我是来寻找古老遗物的。暗影议会即将复苏，我需要力量来对抗他们！",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter8.dialogs.push_back({
        74, "守护者残魂", "暗影议会...那个禁忌的名字...想要获得遗物，必须证明自己的实力。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter8.dialogs.push_back({
        75, "守护者残魂", "击败我的试炼吧，勇士！",
        "npc.png", {}, false, nullptr, nullptr, 1008, 0
    });
    chapter8.dialogs.push_back({
        76, "你", "来吧！",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[8] = chapter8;

    // ===== 第九章：沼泽女巫 =====
    StoryChapter chapter9;
    chapter9.id = 9;
    chapter9.title = "沼泽女巫";
    chapter9.description = "获得遗物后，沼泽深处的女巫似乎有话要说...";
    chapter9.requiredChapterId = 8;

    chapter9.dialogs.push_back({
        80, "守护者残魂", "（消散前）很好...你通过了试炼。这块光明石就交给你了。愿它照亮你前行的道路...",
        "npc.png", {}, false, nullptr, nullptr, 0, 1008
    });
    chapter9.dialogs.push_back({
        81, "你", "（获得光明石）好温暖的力量...",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter9.dialogs.push_back({
        82, "沼泽女巫", "（突然出现在身后）哼，圣殿骑士团的小把戏。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter9.dialogs.push_back({
        83, "你", "你是谁？！",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter9.dialogs.push_back({
        84, "沼泽女巫", "我是这片沼泽的守护者。我看到了你的未来...黑暗与光明交织。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter9.dialogs.push_back({
        85, "沼泽女巫", "想要真正对抗暗影议会，仅靠光明石是不够的。你需要学习古老的魔法。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter9.dialogs.push_back({
        86, "沼泽女巫", "去收集5个暗影碎片给我，我会教你第一招暗影抗性魔法。",
        "npc.png", {}, false, nullptr, nullptr, 1009, 0
    });
    chapter9.dialogs.push_back({
        87, "你", "暗影碎片？在哪里能找到？",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter9.dialogs.push_back({
        88, "沼泽女巫", "暗影狼和魔化蜘蛛身上都有。它们已经被暗影议会污染了。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter9.dialogs.push_back({
        89, "你", "明白了，我会收集到的。",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[9] = chapter9;

    // ===== 第十章：通往王城之路 =====
    StoryChapter chapter10;
    chapter10.id = 10;
    chapter10.title = "通往王城之路";
    chapter10.description = "一切准备就绪，踏上前往王城的道路...";
    chapter10.requiredChapterId = 9;

    chapter10.dialogs.push_back({
        90, "沼泽女巫", "（接过暗影碎片）很好，这些碎片中的暗影能量很纯净。",
        "npc.png", {}, false, nullptr, nullptr, 0, 1009
    });
    chapter10.dialogs.push_back({
        91, "沼泽女巫", "（念咒语）以暗影之力，铸就暗影之盾。从今以后，暗影魔法对你的伤害将大大减弱。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter10.dialogs.push_back({
        92, "你", "（感受到力量涌入）这股力量...谢谢！",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter10.dialogs.push_back({
        93, "沼泽女巫", "别高兴太早。王城的水比你想象的深。圣殿骑士团内部...也有暗影议会的内鬼。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter10.dialogs.push_back({
        94, "你", "什么？！那我把印记交给他们不是自投罗网？",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter10.dialogs.push_back({
        95, "沼泽女巫", "去找大团长阿尔萨斯，只有他是完全可信的。记住，不要轻信任何人。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter10.dialogs.push_back({
        96, "神秘使者", "（再次出现）是时候出发了。王城的道路已经打开。",
        "npc.png", {}, false, nullptr, nullptr, 1010, 0
    });
    chapter10.dialogs.push_back({
        97, "你", "王城...我来了。不管有什么阴谋，我都会揭开真相！",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[10] = chapter10;

    // ===== 第十一章：王城的阴谋 =====
    StoryChapter chapter11;
    chapter11.id = 11;
    chapter11.title = "王城的阴谋";
    chapter11.description = "到达王城，发现暗影议会的阴谋远比想象中更大...";
    chapter11.requiredChapterId = 10;

    chapter11.dialogs.push_back({
        100, "大团长阿尔萨斯", "你就是带来暗影印记的勇士？来得正好。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter11.dialogs.push_back({
        101, "大团长阿尔萨斯", "最近王城地下墓穴频繁出现亡灵活动。我怀疑暗影议会正在尝试复活他们的领袖——暗影君王。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter11.dialogs.push_back({
        102, "大团长阿尔萨斯", "我需要你潜入地下墓穴，阻止他们的仪式。这是王城最后的防线。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter11.dialogs.push_back({
        103, "你", "交给我吧！我不会让他们得逞的！",
        "player.png", {}, false, nullptr, nullptr, 1011, 0
    });
    chapter11.dialogs.push_back({
        104, "大团长阿尔萨斯", "小心，骑士团中可能有人背叛了我们。如果发现异常，立刻撤退。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter11.dialogs.push_back({
        105, "你", "我会小心的。为了这片土地，我一定要成功！",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[11] = chapter11;

    // ===== 第十二章：暗影君王 =====
    StoryChapter chapter12;
    chapter12.id = 12;
    chapter12.title = "暗影君王";
    chapter12.description = "最终决战，面对暗影君王...";
    chapter12.requiredChapterId = 11;

    chapter12.dialogs.push_back({
        110, "你", "（深入地下墓穴）这里...到处弥漫着死亡的气息。",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter12.dialogs.push_back({
        111, "暗影祭祀", "（狂笑）愚蠢的人类！你以为能阻止暗影君王的复活？",
        "monster.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter12.dialogs.push_back({
        112, "暗影祭祀", "仪式已经完成！暗影君王即将降临！",
        "monster.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter12.dialogs.push_back({
        113, "你", "不！我绝不会让你们毁灭这个世界！",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter12.dialogs.push_back({
        114, "暗影君王", "（从黑暗中现身）渺小的凡人...你的勇气值得赞赏。但这改变不了什么。",
        "monster.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter12.dialogs.push_back({
        115, "暗影君王", "让我看看，你究竟有几分实力！",
        "monster.png", {}, false, nullptr, nullptr, 1012, 0
    });
    chapter12.dialogs.push_back({
        116, "你", "光明石，赐予我力量！为了所有人，我一定要赢！",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[12] = chapter12;

    // ===== 终章：新的开始 =====
    StoryChapter chapter13;
    chapter13.id = 13;
    chapter13.title = "新的开始";
    chapter13.description = "击败暗影君王，但新的冒险才刚刚开始...";
    chapter13.requiredChapterId = 12;

    chapter13.dialogs.push_back({
        120, "暗影君王", "（消散）不可能...我怎么会被一个凡人...",
        "monster.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter13.dialogs.push_back({
        121, "你", "（喘息）终于...结束了...",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter13.dialogs.push_back({
        122, "大团长阿尔萨斯", "（赶到）你做到了！你真的击败了暗影君王！",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter13.dialogs.push_back({
        123, "大团长阿尔萨斯", "但暗影议会只是暂时受挫。他们的根源在更遥远的地方——暗影之域。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter13.dialogs.push_back({
        124, "大团长阿尔萨斯", "勇士，你的旅程才刚刚开始。这个世界需要你的守护。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter13.dialogs.push_back({
        125, "你", "不管前方有什么，我都不会退缩。这只是开始！",
        "player.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter13.dialogs.push_back({
        126, "老村长", "（从村口远远望来）孩子...你已经成长了这么多。无论你去哪里，我们永远是你的家。",
        "npc.png", {}, false, nullptr, nullptr, 0, 0
    });
    chapter13.dialogs.push_back({
        127, "你", "谢谢你们。我会回来的，带着和平与希望！",
        "player.png", {}, true, nullptr, nullptr, 0, 0
    });

    m_chapters[13] = chapter13;

    // ===== 设置任务 =====
    setupQuests();
}

void StoryManager::setupQuests() {
    // ===== 主线任务 =====

    // 任务1001：清理史莱姆
    Quest q1;
    q1.id = 1001;
    q1.title = "村外的威胁";
    q1.description = "村外出现了一群史莱姆，袭击了运送草药的村民。老村长请求你清理3只史莱姆。";
    q1.brief = "消灭3只史莱姆";
    q1.type = QuestType::Main;
    q1.startNpc = "老村长";
    q1.endNpc = "老村长";
    q1.requiredLevel = 1;
    q1.preQuestId = 0;

    QuestObjective obj1;
    obj1.id = 1;
    obj1.description = "消灭3只史莱姆";
    obj1.type = QuestObjectiveType::KillMonster;
    obj1.targetId = 1001; // 史莱姆ID
    obj1.requiredCount = 3;
    obj1.locationHint = "村庄东边的草地";
    q1.objectives.push_back(obj1);

    q1.reward.exp = 50;
    q1.reward.gold = 100;
    m_questDatabase[1001] = q1;

    // 任务1002：铁匠的考验
    Quest q2;
    q2.id = 1002;
    q2.title = "铁匠的考验";
    q2.description = "铁匠老张需要矿石，但矿洞被魔化蜘蛛占据。清理5只魔化蜘蛛，换取新武器。";
    q2.brief = "消灭5只魔化蜘蛛";
    q2.type = QuestType::Main;
    q2.startNpc = "铁匠老张";
    q2.endNpc = "铁匠老张";
    q2.requiredLevel = 2;
    q2.preQuestId = 1001;

    QuestObjective obj2;
    obj2.id = 1;
    obj2.description = "消灭5只魔化蜘蛛";
    obj2.type = QuestObjectiveType::KillMonster;
    obj2.targetId = 1003;
    obj2.requiredCount = 5;
    obj2.locationHint = "村庄西边的矿洞";
    q2.objectives.push_back(obj2);

    q2.reward.exp = 100;
    q2.reward.gold = 200;
    q2.reward.rewardEquipment = "铁剑";
    m_questDatabase[1002] = q2;

    // 任务1003：采集草药
    Quest q3;
    q3.id = 1003;
    q3.title = "草药采集";
    q3.description = "药师需要3株龙血草来炼制疗伤药。龙血草生长在沼泽深处。";
    q3.brief = "采集3株龙血草";
    q3.type = QuestType::Main;
    q3.startNpc = "药师";
    q3.endNpc = "药师";
    q3.requiredLevel = 3;
    q3.preQuestId = 1002;

    QuestObjective obj3;
    obj3.id = 1;
    obj3.description = "采集3株龙血草";
    obj3.type = QuestObjectiveType::CollectItem;
    obj3.targetId = 2001; // 龙血草物品ID
    obj3.requiredCount = 3;
    obj3.locationHint = "沼泽深处";
    q3.objectives.push_back(obj3);

    q3.reward.exp = 150;
    q3.reward.gold = 300;
    q3.reward.items.push_back({2002, 5}); // 5瓶小血药
    m_questDatabase[1003] = q3;

    // 任务1004：击败暗影狼（Boss战）
    Quest q4;
    q4.id = 1004;
    q4.title = "暗影的威胁";
    q4.description = "村东废墟出现了一只巨大的暗影狼，已经伤了好几个猎人。击败它保护村庄！";
    q4.brief = "击败暗影狼";
    q4.type = QuestType::Main;
    q4.startNpc = "老村长";
    q4.endNpc = "老村长";
    q4.requiredLevel = 5;
    q4.preQuestId = 1003;

    QuestObjective obj4;
    obj4.id = 1;
    obj4.description = "击败暗影狼";
    obj4.type = QuestObjectiveType::DefeatBoss;
    obj4.targetId = 2001; // 暗影狼ID
    obj4.requiredCount = 1;
    obj4.locationHint = "村东废墟";
    q4.objectives.push_back(obj4);

    q4.reward.exp = 500;
    q4.reward.gold = 1000;
    q4.reward.rewardEquipment = "暗影之刃";
    m_questDatabase[1004] = q4;

    // 任务1005：猎人的委托 - 收集野猪獠牙
    Quest q5;
    q5.id = 1005;
    q5.title = "猎人的考验";
    q5.description = "王猎户需要8个野猪獠牙来制作猎具。野猪在村北森林出没。";
    q5.brief = "收集8个野猪獠牙";
    q5.type = QuestType::Main;
    q5.startNpc = "王猎户";
    q5.endNpc = "王猎户";
    q5.requiredLevel = 6;
    q5.preQuestId = 1004;

    QuestObjective obj5;
    obj5.id = 1;
    obj5.description = "收集8个野猪獠牙";
    obj5.type = QuestObjectiveType::CollectItem;
    obj5.targetId = 3001; // 野猪獠牙
    obj5.requiredCount = 8;
    obj5.locationHint = "村北森林";
    q5.objectives.push_back(obj5);

    q5.reward.exp = 200;
    q5.reward.gold = 400;
    q5.reward.items.push_back({2002, 10}); // 10瓶小血药
    m_questDatabase[1005] = q5;

    // 任务1006：击败野猪王
    Quest q6;
    q6.id = 1006;
    q6.title = "森林之主";
    q6.description = "村北森林的野猪王正在肆虐。击败它，保护猎人们的安全。";
    q6.brief = "击败野猪王";
    q6.type = QuestType::Main;
    q6.startNpc = "王猎户";
    q6.endNpc = "王猎户";
    q6.requiredLevel = 7;
    q6.preQuestId = 1005;

    QuestObjective obj6;
    obj6.id = 1;
    obj6.description = "击败野猪王";
    obj6.type = QuestObjectiveType::DefeatBoss;
    obj6.targetId = 3002; // 野猪王ID
    obj6.requiredCount = 1;
    obj6.locationHint = "村北森林深处";
    q6.objectives.push_back(obj6);

    q6.reward.exp = 600;
    q6.reward.gold = 800;
    q6.reward.rewardEquipment = "猎人之弓";
    m_questDatabase[1006] = q6;

    // 任务1007：寻找光明石
    Quest q7;
    q7.id = 1007;
    q7.title = "光明石的试炼";
    q7.description = "前往矿洞最深处，通过守护者的试炼，获得光明石。";
    q7.brief = "通过矿洞试炼获得光明石";
    q7.type = QuestType::Main;
    q7.startNpc = "神秘使者";
    q7.endNpc = "守护者残魂";
    q7.requiredLevel = 8;
    q7.preQuestId = 1006;

    QuestObjective obj7;
    obj7.id = 1;
    obj7.description = "击败矿洞守护者";
    obj7.type = QuestObjectiveType::DefeatBoss;
    obj7.targetId = 4001; // 矿洞守护者ID
    obj7.requiredCount = 1;
    obj7.locationHint = "矿洞最深处";
    q7.objectives.push_back(obj7);

    QuestObjective obj7b;
    obj7b.id = 2;
    obj7b.description = "获得光明石";
    obj7b.type = QuestObjectiveType::CollectItem;
    obj7b.targetId = 4002; // 光明石
    obj7b.requiredCount = 1;
    obj7b.locationHint = "矿洞最深处祭坛";
    q7.objectives.push_back(obj7b);

    q7.reward.exp = 800;
    q7.reward.gold = 1200;
    q7.reward.rewardEquipment = "光明石护符";
    m_questDatabase[1007] = q7;

    // 任务1008：学习暗影抗性
    Quest q8;
    q8.id = 1008;
    q8.title = "暗影抗性";
    q8.description = "沼泽女巫需要5个暗影碎片来传授你暗影抗性魔法。碎片可以在被污染的魔物身上找到。";
    q8.brief = "收集5个暗影碎片";
    q8.type = QuestType::Main;
    q8.startNpc = "沼泽女巫";
    q8.endNpc = "沼泽女巫";
    q8.requiredLevel = 9;
    q8.preQuestId = 1007;

    QuestObjective obj8;
    obj8.id = 1;
    obj8.description = "收集5个暗影碎片";
    obj8.type = QuestObjectiveType::CollectItem;
    obj8.targetId = 5001; // 暗影碎片
    obj8.requiredCount = 5;
    obj8.locationHint = "击败暗影狼或魔化蜘蛛";
    q8.objectives.push_back(obj8);

    q8.reward.exp = 400;
    q8.reward.gold = 600;
    q8.reward.items.push_back({5002, 1}); // 暗影抗性技能书
    m_questDatabase[1008] = q8;

    // 任务1009：通往王城
    Quest q9;
    q9.id = 1009;
    q9.title = "前往王城";
    q9.description = "一切准备就绪，踏上前往王城的道路。与王城守卫对话进入王城。";
    q9.brief = "到达王城";
    q9.type = QuestType::Main;
    q9.startNpc = "神秘使者";
    q9.endNpc = "王城守卫";
    q9.requiredLevel = 10;
    q9.preQuestId = 1008;

    QuestObjective obj9;
    obj9.id = 1;
    obj9.description = "到达王城";
    obj9.type = QuestObjectiveType::ReachLocation;
    obj9.targetId = 6001; // 王城
    obj9.requiredCount = 1;
    obj9.locationHint = "沿着王城大道前进";
    q9.objectives.push_back(obj9);

    q9.reward.exp = 300;
    q9.reward.gold = 500;
    m_questDatabase[1009] = q9;

    // 任务1010：地下墓穴的危机
    Quest q10;
    q10.id = 1010;
    q10.title = "地下墓穴";
    q10.description = "潜入王城地下墓穴，阻止暗影议会复活暗影君王的仪式。";
    q10.brief = "阻止暗影仪式";
    q10.type = QuestType::Main;
    q10.startNpc = "大团长阿尔萨斯";
    q10.endNpc = "大团长阿尔萨斯";
    q10.requiredLevel = 12;
    q10.preQuestId = 1009;

    QuestObjective obj10;
    obj10.id = 1;
    obj10.description = "击败暗影祭祀";
    obj10.type = QuestObjectiveType::DefeatBoss;
    obj10.targetId = 6002; // 暗影祭祀ID
    obj10.requiredCount = 1;
    obj10.locationHint = "王城地下墓穴最深处";
    q10.objectives.push_back(obj10);

    q10.reward.exp = 1000;
    q10.reward.gold = 2000;
    q10.reward.rewardEquipment = "圣殿骑士铠甲";
    m_questDatabase[1010] = q10;

    // 任务1011：最终决战 - 击败暗影君王
    Quest q11;
    q11.id = 1011;
    q11.title = "最终决战";
    q11.description = "暗影君王即将降临。击败他，拯救世界！";
    q11.brief = "击败暗影君王";
    q11.type = QuestType::Main;
    q11.startNpc = "大团长阿尔萨斯";
    q11.endNpc = "大团长阿尔萨斯";
    q11.requiredLevel = 15;
    q11.preQuestId = 1010;

    QuestObjective obj11;
    obj11.id = 1;
    obj11.description = "击败暗影君王";
    obj11.type = QuestObjectiveType::DefeatBoss;
    obj11.targetId = 6003; // 暗影君王ID
    obj11.requiredCount = 1;
    obj11.locationHint = "地下墓穴祭坛";
    q11.objectives.push_back(obj11);

    q11.reward.exp = 2000;
    q11.reward.gold = 5000;
    q11.reward.rewardEquipment = "光明圣剑";
    q11.reward.items.push_back({6004, 1}); // 王者之冠
    m_questDatabase[1011] = q11;

    // ===== 支线任务 =====

    // 支线任务2001：日常讨伐
    Quest sq1;
    sq1.id = 2001;
    sq1.title = "日常讨伐";
    sq1.description = "为了维护村子安全，消灭任意10只魔物。";
    sq1.brief = "消灭10只魔物";
    sq1.type = QuestType::Daily;
    sq1.startNpc = "老村长";
    sq1.endNpc = "老村长";
    sq1.requiredLevel = 1;
    sq1.preQuestId = 0;
    sq1.isRepeatable = true;

    QuestObjective sobj1;
    sobj1.id = 1;
    sobj1.description = "消灭10只魔物";
    sobj1.type = QuestObjectiveType::KillMonster;
    sobj1.targetId = 0; // 任意怪物
    sobj1.requiredCount = 10;
    sobj1.locationHint = "村外任意区域";
    sq1.objectives.push_back(sobj1);

    sq1.reward.exp = 80;
    sq1.reward.gold = 150;
    m_questDatabase[2001] = sq1;

    // 支线任务2002：铁匠的矿石需求
    Quest sq2;
    sq2.id = 2002;
    sq2.title = "铁矿收集";
    sq2.description = "铁匠老张需要10块铁矿石来补充库存。";
    sq2.brief = "收集10块铁矿石";
    sq2.type = QuestType::Side;
    sq2.startNpc = "铁匠老张";
    sq2.endNpc = "铁匠老张";
    sq2.requiredLevel = 2;
    sq2.preQuestId = 1001;

    QuestObjective sobj2;
    sobj2.id = 1;
    sobj2.description = "收集10块铁矿石";
    sobj2.type = QuestObjectiveType::CollectItem;
    sobj2.targetId = 7001; // 铁矿石
    sobj2.requiredCount = 10;
    sobj2.locationHint = "矿洞各处";
    sq2.objectives.push_back(sobj2);

    sq2.reward.exp = 60;
    sq2.reward.gold = 120;
    sq2.reward.items.push_back({7002, 3}); // 3瓶铁匠药水
    m_questDatabase[2002] = sq2;

    // 支线任务2003：药师的进阶研究
    Quest sq3;
    sq3.id = 2003;
    sq3.title = "稀有药剂";
    sq3.description = "药师需要研究一种新药剂，需要5个史莱姆凝胶和3个蜘蛛毒囊。";
    sq3.brief = "收集5个史莱姆凝胶和3个蜘蛛毒囊";
    sq3.type = QuestType::Side;
    sq3.startNpc = "药师";
    sq3.endNpc = "药师";
    sq3.requiredLevel = 3;
    sq3.preQuestId = 1001;

    QuestObjective sobj3a;
    sobj3a.id = 1;
    sobj3a.description = "收集5个史莱姆凝胶";
    sobj3a.type = QuestObjectiveType::CollectItem;
    sobj3a.targetId = 7003; // 史莱姆凝胶
    sobj3a.requiredCount = 5;
    sobj3a.locationHint = "村东草地击败史莱姆";
    sq3.objectives.push_back(sobj3a);

    QuestObjective sobj3b;
    sobj3b.id = 2;
    sobj3b.description = "收集3个蜘蛛毒囊";
    sobj3b.type = QuestObjectiveType::CollectItem;
    sobj3b.targetId = 7004; // 蜘蛛毒囊
    sobj3b.requiredCount = 3;
    sobj3b.locationHint = "村西矿洞击败魔化蜘蛛";
    sq3.objectives.push_back(sobj3b);

    sq3.reward.exp = 120;
    sq3.reward.gold = 250;
    sq3.reward.items.push_back({2003, 5}); // 5瓶大血药
    m_questDatabase[2003] = sq3;

    // 支线任务2004：猎人大师的挑战
    Quest sq4;
    sq4.id = 2004;
    sq4.title = "猎人大师";
    sq4.description = "王猎户想要测试你的真正实力。在30分钟内击败20只任意魔物。";
    sq4.brief = "30分钟内击败20只魔物";
    sq4.type = QuestType::Side;
    sq4.startNpc = "王猎户";
    sq4.endNpc = "王猎户";
    sq4.requiredLevel = 5;
    sq4.preQuestId = 1004;

    QuestObjective sobj4;
    sobj4.id = 1;
    sobj4.description = "30分钟内击败20只魔物";
    sobj4.type = QuestObjectiveType::KillMonster;
    sobj4.targetId = 0; // 任意
    sobj4.requiredCount = 20;
    sobj4.locationHint = "村外任意区域";
    sq4.objectives.push_back(sobj4);

    sq4.reward.exp = 250;
    sq4.reward.gold = 500;
    sq4.reward.rewardEquipment = "猎人徽章";
    m_questDatabase[2004] = sq4;

    // 支线任务2005：失落的宝藏
    Quest sq5;
    sq5.id = 2005;
    sq5.title = "失落的宝藏";
    sq5.description = "老村长年轻时在王城附近埋藏了一批宝藏。去挖出来吧！";
    sq5.brief = "在王城郊外的古树下挖掘宝藏";
    sq5.type = QuestType::Side;
    sq5.startNpc = "老村长";
    sq5.endNpc = "老村长";
    sq5.requiredLevel = 10;
    sq5.preQuestId = 1009;

    QuestObjective sobj5;
    sobj5.id = 1;
    sobj5.description = "在王城郊外的古树下挖掘";
    sobj5.type = QuestObjectiveType::ReachLocation;
    sobj5.targetId = 8001; // 古树
    sobj5.requiredCount = 1;
    sobj5.locationHint = "王城外东侧古树下";
    sq5.objectives.push_back(sobj5);

    sq5.reward.exp = 500;
    sq5.reward.gold = 2000;
    sq5.reward.items.push_back({8002, 1}); // 神秘宝箱
    m_questDatabase[2005] = sq5;

    // 支线任务2006：村子的防御
    Quest sq6;
    sq6.id = 2006;
    sq6.title = "村子的防御";
    sq6.description = "加固村子围墙需要20块木材。去森林砍伐收集吧。";
    sq6.brief = "收集20块木材";
    sq6.type = QuestType::Side;
    sq6.startNpc = "老村长";
    sq6.endNpc = "老村长";
    sq6.requiredLevel = 1;
    sq6.preQuestId = 0;

    QuestObjective sobj6;
    sobj6.id = 1;
    sobj6.description = "收集20块木材";
    sobj6.type = QuestObjectiveType::CollectItem;
    sobj6.targetId = 9001; // 木材
    sobj6.requiredCount = 20;
    sobj6.locationHint = "村北森林";
    sq6.objectives.push_back(sobj6);

    sq6.reward.exp = 40;
    sq6.reward.gold = 80;
    sq6.reward.items.push_back({2002, 3}); // 3瓶小血药
    m_questDatabase[2006] = sq6;
}

void StoryManager::setupChapters() {
    // Chapters already set up in setupDefaultStory
}

// ===== 任务管理 =====

void StoryManager::addQuest(const Quest& quest) {
    m_questDatabase[quest.id] = quest;
}

void StoryManager::startQuest(int questId) {
    auto it = m_questDatabase.find(questId);
    if (it == m_questDatabase.end()) return;

    // 检查前置任务
    if (it->second.preQuestId > 0 && !isQuestCompleted(it->second.preQuestId)) {
        log("[Story] Cannot start quest %d, pre-quest %d not completed", questId, it->second.preQuestId);
        return;
    }

    // 检查等级
    auto player = Player::getInstance();
    if (player->getLevel() < it->second.requiredLevel) {
        log("[Story] Level too low for quest %d", questId);
        return;
    }

    // 添加到活跃任务
    Quest quest = it->second;
    quest.state = QuestState::Active;
    m_activeQuests.push_back(quest);

    // 从可接任务中移除
    auto availIt = std::find_if(m_availableQuests.begin(), m_availableQuests.end(),
        [questId](const Quest& q) { return q.id == questId; });
    if (availIt != m_availableQuests.end()) {
        m_availableQuests.erase(availIt);
    }

    notifyEvent(StoryEventType::QuestStart, questId);
    log("[Story] Quest started: %s", quest.title.c_str());
}

void StoryManager::completeQuestObjective(int questId, int objectiveId) {
    for (auto& quest : m_activeQuests) {
        if (quest.id == questId) {
            for (auto& obj : quest.objectives) {
                if (obj.id == objectiveId && !obj.completed) {
                    obj.currentCount++;
                    if (obj.currentCount >= obj.requiredCount) {
                        obj.completed = true;
                    }
                }
            }
            checkQuestCompletion(quest);
            return;
        }
    }
}

void StoryManager::finishQuest(int questId) {
    auto it = std::find_if(m_activeQuests.begin(), m_activeQuests.end(),
        [questId](const Quest& q) { return q.id == questId; });
    if (it == m_activeQuests.end()) return;

    if (it->state != QuestState::Completed) {
        log("[Story] Quest %d not completed yet", questId);
        return;
    }

    // 发放奖励
    auto player = Player::getInstance();
    player->addExp(it->reward.exp);
    // gold would be added to a currency system if exists

    // 物品奖励
    for (const auto& item : it->reward.items) {
        // Would add to inventory via item database
    }

    // 移动到已完成
    it->state = QuestState::Finished;
    m_completedQuests.push_back(*it);
    m_activeQuests.erase(it);

    notifyEvent(StoryEventType::QuestComplete, questId);
    log("[Story] Quest finished: %d, reward: %d exp", questId, it->reward.exp);

    // 检查是否有后续章节解锁
    updateAvailableQuests();
}

void StoryManager::abandonQuest(int questId) {
    auto it = std::find_if(m_activeQuests.begin(), m_activeQuests.end(),
        [questId](const Quest& q) { return q.id == questId; });
    if (it != m_activeQuests.end()) {
        it->state = QuestState::NotStarted;
        m_availableQuests.push_back(*it);
        m_activeQuests.erase(it);
    }
}

Quest* StoryManager::getQuest(int questId) {
    for (auto& q : m_activeQuests) {
        if (q.id == questId) return &q;
    }
    for (auto& q : m_availableQuests) {
        if (q.id == questId) return &q;
    }
    for (auto& q : m_completedQuests) {
        if (q.id == questId) return &q;
    }
    auto it = m_questDatabase.find(questId);
    return it != m_questDatabase.end() ? &it->second : nullptr;
}

bool StoryManager::hasQuest(int questId) const {
    for (const auto& q : m_activeQuests) {
        if (q.id == questId) return true;
    }
    return false;
}

bool StoryManager::isQuestCompleted(int questId) const {
    for (const auto& q : m_completedQuests) {
        if (q.id == questId) return true;
    }
    return false;
}

bool StoryManager::isQuestActive(int questId) const {
    for (const auto& q : m_activeQuests) {
        if (q.id == questId) return true;
    }
    return false;
}

void StoryManager::checkQuestCompletion(Quest& quest) {
    bool allCompleted = true;
    for (const auto& obj : quest.objectives) {
        if (!obj.completed) {
            allCompleted = false;
            break;
        }
    }
    if (allCompleted && quest.state == QuestState::Active) {
        quest.state = QuestState::Completed;
        log("[Story] Quest completed: %s", quest.title.c_str());
    }
}

void StoryManager::updateAvailableQuests() {
    m_availableQuests.clear();
    for (const auto& pair : m_questDatabase) {
        const auto& quest = pair.second;
        // 不在进行中且未完成的可接任务
        if (!hasQuest(quest.id) && !isQuestCompleted(quest.id)) {
            if (quest.preQuestId == 0 || isQuestCompleted(quest.preQuestId)) {
                auto player = Player::getInstance();
                if (player->getLevel() >= quest.requiredLevel) {
                    m_availableQuests.push_back(quest);
                }
            }
        }
    }
}

// ===== 任务进度更新 =====

void StoryManager::updateKillObjective(int monsterId, int count) {
    for (auto& quest : m_activeQuests) {
        for (auto& obj : quest.objectives) {
            if (obj.type == QuestObjectiveType::KillMonster && !obj.completed) {
                if (obj.targetId == 0 || obj.targetId == monsterId) {
                    obj.currentCount = std::min(obj.currentCount + count, obj.requiredCount);
                    if (obj.currentCount >= obj.requiredCount) {
                        obj.completed = true;
                    }
                }
            }
        }
        checkQuestCompletion(quest);
    }
}

void StoryManager::updateCollectObjective(int itemId, int count) {
    for (auto& quest : m_activeQuests) {
        for (auto& obj : quest.objectives) {
            if (obj.type == QuestObjectiveType::CollectItem && !obj.completed) {
                if (obj.targetId == itemId) {
                    obj.currentCount = std::min(obj.currentCount + count, obj.requiredCount);
                    if (obj.currentCount >= obj.requiredCount) {
                        obj.completed = true;
                    }
                }
            }
        }
        checkQuestCompletion(quest);
    }
}

void StoryManager::updateTalkObjective(int npcId) {
    for (auto& quest : m_activeQuests) {
        for (auto& obj : quest.objectives) {
            if (obj.type == QuestObjectiveType::TalkToNPC && !obj.completed) {
                if (obj.targetId == npcId) {
                    obj.currentCount++;
                    if (obj.currentCount >= obj.requiredCount) {
                        obj.completed = true;
                    }
                }
            }
        }
        checkQuestCompletion(quest);
    }
}

void StoryManager::updateReachLocation(int locationId) {
    for (auto& quest : m_activeQuests) {
        for (auto& obj : quest.objectives) {
            if (obj.type == QuestObjectiveType::ReachLocation && !obj.completed) {
                if (obj.targetId == locationId) {
                    obj.currentCount++;
                    if (obj.currentCount >= obj.requiredCount) {
                        obj.completed = true;
                    }
                }
            }
        }
        checkQuestCompletion(quest);
    }
}

void StoryManager::updateSkillUse(int skillId) {
    for (auto& quest : m_activeQuests) {
        for (auto& obj : quest.objectives) {
            if (obj.type == QuestObjectiveType::UseSkill && !obj.completed) {
                if (obj.targetId == skillId) {
                    obj.currentCount++;
                    if (obj.currentCount >= obj.requiredCount) {
                        obj.completed = true;
                    }
                }
            }
        }
        checkQuestCompletion(quest);
    }
}

void StoryManager::updateLevelUp(int level) {
    for (auto& quest : m_activeQuests) {
        for (auto& obj : quest.objectives) {
            if (obj.type == QuestObjectiveType::LevelUp && !obj.completed) {
                if (level >= obj.targetId) {
                    obj.currentCount = obj.requiredCount;
                    obj.completed = true;
                }
            }
        }
        checkQuestCompletion(quest);
    }
}

// ===== 对话系统 =====

void StoryManager::startDialog(int chapterId, int startNodeId) {
    auto it = m_chapters.find(chapterId);
    if (it == m_chapters.end()) return;

    m_currentDialogChapterId = chapterId;
    m_currentDialogSequence = it->second.dialogs;
    m_currentDialogIndex = startNodeId;

    // 找到起始节点
    if (startNodeId > 0) {
        for (size_t i = 0; i < m_currentDialogSequence.size(); ++i) {
            if (m_currentDialogSequence[i].id == startNodeId) {
                m_currentDialogIndex = static_cast<int>(i);
                break;
            }
        }
    } else {
        m_currentDialogIndex = 0;
    }

    notifyEvent(StoryEventType::DialogTrigger, chapterId);

    auto& node = m_currentDialogSequence[m_currentDialogIndex];
    if (node.onStart) node.onStart();
}

void StoryManager::showNextDialog() {
    if (!isDialogActive()) return;

    auto& current = m_currentDialogSequence[m_currentDialogIndex];

    // 如果有选择，不自动推进
    if (!current.choices.empty()) return;

    if (current.onEnd) current.onEnd();

    if (current.isEnd) {
        m_currentDialogIndex = -1;
        m_currentDialogChapterId = -1;
        return;
    }

    m_currentDialogIndex++;
    if (m_currentDialogIndex >= static_cast<int>(m_currentDialogSequence.size())) {
        m_currentDialogIndex = -1;
        m_currentDialogChapterId = -1;
        return;
    }

    auto& next = m_currentDialogSequence[m_currentDialogIndex];
    if (next.onStart) next.onStart();
}

void StoryManager::selectChoice(int choiceIndex) {
    if (!isDialogActive()) return;

    auto& current = m_currentDialogSequence[m_currentDialogIndex];
    if (choiceIndex < 0 || choiceIndex >= static_cast<int>(current.choices.size())) return;

    int nextNodeId = current.choices[choiceIndex].first;

    // 找到下一节点
    for (size_t i = 0; i < m_currentDialogSequence.size(); ++i) {
        if (m_currentDialogSequence[i].id == nextNodeId) {
            m_currentDialogIndex = static_cast<int>(i);
            auto& next = m_currentDialogSequence[i];
            if (next.onStart) next.onStart();
            return;
        }
    }
}

const DialogNode* StoryManager::getCurrentDialog() const {
    if (!isDialogActive()) return nullptr;
    if (m_currentDialogIndex < 0 || m_currentDialogIndex >= static_cast<int>(m_currentDialogSequence.size()))
        return nullptr;
    return &m_currentDialogSequence[m_currentDialogIndex];
}

// ===== 章节管理 =====

void StoryManager::startChapter(int chapterId) {
    auto it = m_chapters.find(chapterId);
    if (it == m_chapters.end()) return;

    if (it->second.requiredChapterId > 0 && !isChapterCompleted(it->second.requiredChapterId)) {
        log("[Story] Cannot start chapter %d, required %d not completed", chapterId, it->second.requiredChapterId);
        return;
    }

    m_currentChapterId = chapterId;
    startDialog(chapterId);
    notifyEvent(StoryEventType::ChapterStart, chapterId);
}

void StoryManager::completeChapter(int chapterId) {
    auto it = m_chapters.find(chapterId);
    if (it != m_chapters.end()) {
        it->second.isCompleted = true;
        notifyEvent(StoryEventType::ChapterComplete, chapterId);
    }
}

bool StoryManager::isChapterCompleted(int chapterId) const {
    auto it = m_chapters.find(chapterId);
    return it != m_chapters.end() && it->second.isCompleted;
}

const StoryChapter* StoryManager::getChapter(int chapterId) const {
    auto it = m_chapters.find(chapterId);
    return it != m_chapters.end() ? &it->second : nullptr;
}

int StoryManager::getCurrentChapterNumber() const {
    int count = 0;
    for (const auto& pair : m_chapters) {
        if (pair.second.isCompleted) count++;
    }
    return count;
}

std::string StoryManager::getStorySummary() const {
    std::string summary = "=== 剧情进度 ===\n";
    for (const auto& pair : m_chapters) {
        const auto& ch = pair.second;
        summary += ch.title + " [" + (ch.isCompleted ? "已完成" : "未完成") + "]\n";
    }
    summary += "\n=== 已完成任务 ===\n";
    for (const auto& q : m_completedQuests) {
        summary += q.title + "\n";
    }
    summary += "\n=== 进行中任务 ===\n";
    for (const auto& q : m_activeQuests) {
        summary += q.title + "\n";
    }
    return summary;
}

// ===== 事件监听 =====

void StoryManager::addEventListener(const std::string& name, StoryEventCallback callback) {
    m_eventListeners[name] = callback;
}

void StoryManager::removeEventListener(const std::string& name) {
    m_eventListeners.erase(name);
}

void StoryManager::notifyEvent(StoryEventType type, int id) {
    for (const auto& pair : m_eventListeners) {
        pair.second(type, id);
    }
}

// ===== 保存/加载 =====

void StoryManager::saveProgress() {
    std::string json = "{";
    json += "\"completed_chapters\":[";
    bool first = true;
    for (const auto& pair : m_chapters) {
        if (pair.second.isCompleted) {
            if (!first) json += ",";
            json += std::to_string(pair.first);
            first = false;
        }
    }
    json += "],";

    json += "\"completed_quests\":[";
    first = true;
    for (const auto& q : m_completedQuests) {
        if (!first) json += ",";
        json += std::to_string(q.id);
        first = false;
    }
    json += "],";

    json += "\"active_quests\":[";
    first = true;
    for (const auto& q : m_activeQuests) {
        if (!first) json += ",";
        json += "{";
        json += "\"id\":" + std::to_string(q.id) + ",";
        json += "\"objectives\":[";
        bool firstObj = true;
        for (const auto& obj : q.objectives) {
            if (!firstObj) json += ",";
            json += "{";
            json += "\"id\":" + std::to_string(obj.id) + ",";
            json += "\"count\":" + std::to_string(obj.currentCount);
            json += "}";
            firstObj = false;
        }
        json += "]}";
        first = false;
    }
    json += "]}";

    auto path = FileUtils::getInstance()->getWritablePath() + m_saveFile;
    FileUtils::getInstance()->writeStringToFile(json, path);
    log("[Story] Progress saved to %s", path.c_str());
}

void StoryManager::loadProgress() {
    auto path = FileUtils::getInstance()->getWritablePath() + m_saveFile;
    if (!FileUtils::getInstance()->isFileExist(path)) {
        log("[Story] No save file found, starting fresh");
        return;
    }

    auto data = FileUtils::getInstance()->getStringFromFile(path);
    if (data.empty()) return;

    // Simple parse - load completed chapters
    size_t pos = data.find("\"completed_chapters\":[");
    if (pos != std::string::npos) {
        pos += 22;
        size_t end = data.find("]", pos);
        if (end != std::string::npos) {
            std::string chapters = data.substr(pos, end - pos);
            size_t cpos = 0;
            while (cpos < chapters.size()) {
                size_t comma = chapters.find(",", cpos);
                std::string idStr = chapters.substr(cpos, comma != std::string::npos ? comma - cpos : chapters.size() - cpos);
                int chId = atoi(idStr.c_str());
                if (chId > 0) {
                    auto it = m_chapters.find(chId);
                    if (it != m_chapters.end()) it->second.isCompleted = true;
                }
                if (comma == std::string::npos) break;
                cpos = comma + 1;
            }
        }
    }

    // Parse completed quests
    pos = data.find("\"completed_quests\":[");
    if (pos != std::string::npos) {
        pos += 20;
        size_t end = data.find("]", pos);
        if (end != std::string::npos) {
            std::string quests = data.substr(pos, end - pos);
            size_t qpos = 0;
            while (qpos < quests.size()) {
                size_t comma = quests.find(",", qpos);
                std::string idStr = quests.substr(qpos, comma != std::string::npos ? comma - qpos : quests.size() - qpos);
                int qId = atoi(idStr.c_str());
                if (qId > 0) {
                    auto it = m_questDatabase.find(qId);
                    if (it != m_questDatabase.end()) {
                        Quest q = it->second;
                        q.state = QuestState::Finished;
                        m_completedQuests.push_back(q);
                    }
                }
                if (comma == std::string::npos) break;
                qpos = comma + 1;
            }
        }
    }

    updateAvailableQuests();
    log("[Story] Progress loaded - %zu completed quests", m_completedQuests.size());
}

void StoryManager::resetProgress() {
    m_activeQuests.clear();
    m_completedQuests.clear();
    for (auto& pair : m_chapters) {
        pair.second.isCompleted = false;
    }
    m_currentChapterId = 0;
    m_currentDialogIndex = -1;
    m_currentDialogChapterId = -1;
    updateAvailableQuests();
    saveProgress();
}
