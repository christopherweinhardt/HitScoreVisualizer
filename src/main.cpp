#include "PluginConfig.hpp"
#include "main.hpp"
<<<<<<< Updated upstream
#include "HsvFlyingScoreEffect.hpp"
=======
>>>>>>> Stashed changes
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/CanvasScaler.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/CanvasRenderer.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Mathf.hpp"
#include "UnityEngine/SpriteRenderer.hpp"

#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/CurvedCanvasSettings.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/TableView.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "TMPro/TextMeshPro.hpp"

#include "GlobalNamespace/LevelCollectionTableView.hpp"
#include "GlobalNamespace/HapticFeedbackController.hpp"
#include "GlobalNamespace/EffectPoolsManualInstaller.hpp"
#include "GlobalNamespace/FlyingScoreEffect.hpp"
#include "GlobalNamespace/FlyingObjectEffect.hpp"
<<<<<<< Updated upstream
=======
#include "GlobalNamespace/FlyingScoreEffect_Pool.hpp"
#include "GlobalNamespace/NoteCutInfo.hpp"
#include "GlobalNamespace/ISaberSwingRatingCounter.hpp"
#include "GlobalNamespace/ScoreModel.hpp"
#include "GlobalNamespace/FlyingScoreSpawner.hpp"
#include "GlobalNamespace/BeatmapObjectExecutionRatingsRecorder_CutScoreHandler.hpp"
#include "GlobalNamespace/ColorExtensions.hpp"
>>>>>>> Stashed changes

#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "Zenject/DiContainer.hpp"
#include <iostream>
#include <string>
<<<<<<< Updated upstream
=======
#include <utility>
#include <iomanip>
#include <sstream>
>>>>>>> Stashed changes

using namespace std;
using namespace HMUI;
using namespace GlobalNamespace;
using namespace QuestUI;
using namespace UnityEngine;

<<<<<<< Updated upstream
DEFINE_TYPE(HitScore, Main);
=======
HSVConfig HSV::config;
bool HSV::configValid = false;
>>>>>>> Stashed changes

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

#define HANDLE_CONFIG_FAILURE(logger, condition) do { \
    if (!condition) { \
        logger.error("Config failed to load properly!"); \
        configValid = false; \
    } else { \
        configValid = true; \
    } \
} while(0)

const ModInfo& getModInfo() {
    return modInfo;
}

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration configuration(modInfo);
    return configuration;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load();
    getPluginConfig().Init(info);
    getConfig().Reload();
    getConfig().Write();

<<<<<<< Updated upstream
    getLogger().info("Completed setup!");
}

=======
FlyingScoreEffect* currentEffect = nullptr;
std::map<GlobalNamespace::ISaberSwingRatingCounter*, swingRatingCounter_context> swingRatingMap;

void JudgeNoContext(FlyingScoreEffect* self, NoteCutInfo& noteCutInfo) {
    int before;
    int after;
    int accuracy;
    ScoreModel::RawScoreWithoutMultiplier(noteCutInfo.swingRatingCounter, noteCutInfo.cutDistanceToCenter, byref(before), byref(after), byref(accuracy));
    int total = before + after + accuracy;
    float timeDependence = std::abs(noteCutInfo.cutNormal.z);
    HSV::checkJudgments(self, total, before, after, accuracy, timeDependence);
    //noteCutInfo.swingRatingCounter->UnregisterDidFinishReceiver(reinterpret_cast<GlobalNamespace::ISaberSwingRatingCounterDidFinishReceiver*>(hsvRatingCounter));
}

void Judge(ISaberSwingRatingCounter* counter) {

    auto itr = swingRatingMap.find(counter);
    if(itr == swingRatingMap.end()) {
        getLogger().info("counter was not found in swingRatingMap!");
        return;
    }
    auto context = itr->second;

    int before;
    int after;
    int accuracy;


    ScoreModel::RawScoreWithoutMultiplier(context.noteCutInfo.swingRatingCounter, context.noteCutInfo.cutDistanceToCenter, byref(before), byref(after), byref(accuracy));
    int total = before + after + accuracy;
    float timeDependence = std::abs(context.noteCutInfo.cutNormal.z);
    if(context.flyingScoreEffect) {
        //context.flyingScoreEffect->maxCutDistanceScoreIndicator->set_enabled(true);
        HSV::checkJudgments(context.flyingScoreEffect, total, before, after, accuracy, timeDependence);
        swingRatingMap.erase(itr);
    }
    //noteCutInfo.swingRatingCounter->UnregisterDidFinishReceiver(reinterpret_cast<GlobalNamespace::ISaberSwingRatingCounterDidFinishReceiver*>(hsvRatingCounter));
}

MAKE_HOOK_MATCH(InitFlyingScoreEffect, &FlyingScoreEffect::InitAndPresent, void, FlyingScoreEffect* self, ByRef<NoteCutInfo> noteCutInfo, int multiplier, float duration, Vector3 targetPos, Quaternion rotation, Color color) {
    if(getPluginConfig().IsEnabled.GetValue()) {
        if(HSV::config.useFixedPos) {

            targetPos.x = HSV::config.fixedPosX;
            targetPos.y = HSV::config.fixedPosY;
            targetPos.z = HSV::config.fixedPosZ;

            auto transform = self->get_transform();
            transform->set_position(targetPos);


            if(currentEffect) {
                //currentEffect->duration = 0;
                getLogger().info("remove effect");
            }
            currentEffect = self;

            getLogger().info("new current effect set");
        }
    }

    InitFlyingScoreEffect(self, noteCutInfo, multiplier, duration, targetPos, rotation, color);

    if(getPluginConfig().IsEnabled.GetValue()) {
        //duration = 0.01;
        self->text->set_text(il2cpp_utils::newcsstr(""));
        self->maxCutDistanceScoreIndicator->set_enabled(false);

        //
        //hsvRatingCounter->swingRatingCounterFunction = [=](GlobalNamespace::ISaberSwingRatingCounter* saber) { Judge(self, noteCutInfo.heldRef, hsvRatingCounter); };

        JudgeNoContext(self, noteCutInfo.heldRef);

        auto counter = noteCutInfo.heldRef.swingRatingCounter;
>>>>>>> Stashed changes


MAKE_HOOK_MATCH(FlyingScoreEffectHook, &GlobalNamespace::EffectPoolsManualInstaller::ManualInstallBindings, void, GlobalNamespace::EffectPoolsManualInstaller* self, Zenject::DiContainer* diContainer, bool shortBeatEffect) {
    GameObject* gameObject = self->flyingScoreEffectPrefab->get_gameObject();

    FlyingScoreEffect* flyingScoreEffect = gameObject->GetComponent<FlyingScoreEffect*>();

    HitScore::HsvFlyingScoreEffect* hsvFlyingScoreEffect = gameObject->GetComponent<HitScore::HsvFlyingScoreEffect*>();

<<<<<<< Updated upstream
    if(gameObject != nullptr) {
        if(!hsvFlyingScoreEffect) {
            HitScore::HsvFlyingScoreEffect* hsvFlyingScoreEffect = gameObject->AddComponent<HitScore::HsvFlyingScoreEffect*>();

            FlyingObjectEffect* flyingObjectEffect = flyingScoreEffect;

            if(flyingScoreEffect != nullptr) {
                getLogger().info((flyingScoreEffect->moveAnimationCurve ? "move anim true": "false"));
                getLogger().info((flyingScoreEffect->shakeFrequency ? "shake freq true": "false"));
                getLogger().info((flyingScoreEffect->shakeStrength ? "shake streng true": "false"));
                getLogger().info((flyingScoreEffect->shakeStrengthAnimationCurve ? "shake anim true": "false"));
                getLogger().info((flyingScoreEffect->fadeAnimationCurve ? "fade anim true": "false"));
                getLogger().info((flyingScoreEffect->text ? "text true": "false"));
                getLogger().info((flyingScoreEffect->maxCutDistanceScoreIndicator ? "max cut true": "false"));

                getLogger().info((hsvFlyingScoreEffect->moveAnimationCurve ? "move anim HSV": "false"));
                getLogger().info((hsvFlyingScoreEffect->shakeFrequency ? "shake freq HSV": "false"));
                getLogger().info((hsvFlyingScoreEffect->shakeStrength ? "shake streng HSV": "false"));
                getLogger().info((hsvFlyingScoreEffect->shakeStrengthAnimationCurve ? "shake anim HSV": "false"));
                getLogger().info((hsvFlyingScoreEffect->fadeAnimationCurve ? "fade anim HSV": "false"));
                getLogger().info((hsvFlyingScoreEffect->text ? "text HSV": "false"));
                getLogger().info((hsvFlyingScoreEffect->maxCutDistanceScoreIndicator ? "max cut HSV": "false"));

                hsvFlyingScoreEffect->moveAnimationCurve = flyingScoreEffect->moveAnimationCurve;
                hsvFlyingScoreEffect->shakeFrequency = flyingScoreEffect->shakeFrequency;
                hsvFlyingScoreEffect->shakeStrength = flyingScoreEffect->shakeStrength;
                hsvFlyingScoreEffect->shakeStrengthAnimationCurve = flyingScoreEffect->shakeStrengthAnimationCurve;
                hsvFlyingScoreEffect->fadeAnimationCurve = flyingScoreEffect->fadeAnimationCurve;
                hsvFlyingScoreEffect->text = flyingScoreEffect->text;
                hsvFlyingScoreEffect->maxCutDistanceScoreIndicator = flyingScoreEffect->maxCutDistanceScoreIndicator;

                HitScore::JudgmentService* judgmentService = gameObject->AddComponent<HitScore::JudgmentService*>();

                hsvFlyingScoreEffect->Construct(judgmentService);

                /*if(flyingScoreEffect->moveAnimationCurve != nullptr)
                    hsvFlyingScoreEffect->_set__moveAnimationCurve(flyingObjectEffect->moveAnimationCurve);
                if(flyingScoreEffect->shakeFrequency)
                    hsvFlyingScoreEffect->_set__shakeFrequency(flyingObjectEffect->shakeFrequency);
                if(flyingScoreEffect->shakeStrength)
                    hsvFlyingScoreEffect->_set__shakeStrength(flyingObjectEffect->shakeStrength);
                if(flyingScoreEffect->shakeStrengthAnimationCurve != nullptr)
                    hsvFlyingScoreEffect->_set__shakeStrengthAnimationCurve(flyingObjectEffect->shakeStrengthAnimationCurve);

                if(flyingScoreEffect->text != nullptr)
                    hsvFlyingScoreEffect->_set__text(flyingScoreEffect->text);
                if(flyingScoreEffect->fadeAnimationCurve != nullptr)
                    hsvFlyingScoreEffect->_set__fadeAnimationCurve(flyingScoreEffect->fadeAnimationCurve);
                if(flyingScoreEffect->maxCutDistanceScoreIndicator != nullptr)
                    hsvFlyingScoreEffect->_set__maxCutDistanceScoreIndicator(flyingScoreEffect->maxCutDistanceScoreIndicator);
                    */
            }
=======
        if(flyingScoreEffect != nullptr) {
            //judgmentService = CRASH_UNLESS(il2cpp_utils::New<HitScore::JudgmentService*>());
>>>>>>> Stashed changes
        }
    }

    FlyingScoreEffectHook(self, diContainer, shortBeatEffect);
}

void HitScore::Main::Update() {

}

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    getLogger().info("DidActivate: %p, %d, %d, %d", self, firstActivation, addedToHierarchy, screenSystemEnabling);


    /*
    if(firstActivation) {
        self->get_gameObject()->AddComponent<HMUI::Touchable*>();
        self->get_gameObject()->AddComponent<HitScore::Main*>();
        GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());

        auto* textGrid = container;
        //        textGrid->set_spacing(1);

        QuestUI::BeatSaberUI::CreateText(textGrid->get_transform(), "Faster Scroll Mod settings.");
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(textGrid->get_transform(), getPluginConfig().IsStock)->get_gameObject(),"Toggles whether the mod is active or not.");

        //        buttonsGrid->set_spacing(1);

        auto* floatGrid = container;

        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(floatGrid->get_transform(), getPluginConfig().MaxSpeed, 0, 10, 30, 1000)->get_gameObject(),"Changes the speed of scrolling with the joystick. Default: 600");
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(floatGrid->get_transform(), getPluginConfig().Accel, 1, 0.1, 0.5, 5)->get_gameObject(),"Changes the acceleration speed of scrolling with the joystick. Default: 1.5");
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(floatGrid->get_transform(), getPluginConfig().IsLinear)->get_gameObject(),"Toggles whether you want to use acceleration or not.");
    }
     */
}

void HSV::loadConfig() {
    static auto logger = getLogger().WithContext("HSV").WithContext("loadConfig");
    logger.info("Loading Configuration...");
    getConfig().Load();
    HANDLE_CONFIG_FAILURE(logger, ConfigHelper::LoadConfig(config, getConfig().config));
    if (!configValid && config.isDefaultConfig) {
        logger.debug("Setting to default because config failed to load, even though it was default config!");
        config.SetToDefault();
        config.WriteToConfig(getConfig().config);
        getConfig().Write();
        configValid = true;
    }
    if (configValid) {
        logger.info("Loaded Configuration!");
    } else {
        logger.info("Configuration is invalid! Please ensure the config is the correct format!");
    }
}

std::optional<int> HSV::getBestJudgment(std::vector<Judgment> &judgments, int comparison) {
    static auto length = config.judgments.size();
    if (length == 0) {
        return std::nullopt;
    }
    int i;
    for (i = 0; i < length; i++) {
        if (comparison >= config.judgments[i].threshold) {
            break;
        }
    }
    return i;
}

std::optional<const Segment> HSV::getBestSegment(std::vector<Segment>& segments, int comparison) {
    auto size = segments.size();
    if (size == 0) {
        return std::nullopt;
    }
    int bestIdx = 0;
    for (auto& s : segments) {
        getLogger().debug("Segment: %s, %u", s.text.value_or(std::string()).c_str(), s.threshold);
    }
    for (int i = 1; i < size; i++) {
        if (comparison >= segments[bestIdx].threshold) {
            break;
        }
        bestIdx = i;
    }
    getLogger().debug("Got best segment: %s for: %u", segments[bestIdx].text.value_or("").c_str(), comparison);
    return segments[bestIdx];
}

std::optional<const TimeSegment> HSV::getBestTimeSegment(std::vector<TimeSegment>& segments, int comparison) {
    auto size = segments.size();
    if (size == 0) {
        return std::nullopt;
    }
    int bestIdx = 0;
    for (auto& s : segments) {
        getLogger().debug("Segment: %s, %f", s.text.value_or(std::string()).c_str(), s.threshold);
    }
    for (int i = 1; i < size; i++) {
        if (comparison >= segments[bestIdx].threshold) {
            break;
        }
        bestIdx = i;
    }
    getLogger().debug("Got best segment: %s for: %u", segments[bestIdx].text.value_or("").c_str(), comparison);
    return segments[bestIdx];
}

void HSV::checkJudgments(GlobalNamespace::FlyingScoreEffect* flyingScoreEffect, int total, int before, int after, int accuracy, float timeDependence) {
    if(!configValid)
        return;
    auto bestIndex = getBestJudgment(config.judgments, total);
    auto best = config.judgments[bestIndex.value()];

    flyingScoreEffect->text->set_text(il2cpp_utils::newcsstr(JudgeText(total, before, after, accuracy, timeDependence, bestIndex.value())));
    flyingScoreEffect->text->set_color(JudgeColor(total, before, after, accuracy, timeDependence, bestIndex.value()));
    flyingScoreEffect->text->set_richText(true);
    flyingScoreEffect->text->set_enableWordWrapping(false);
    flyingScoreEffect->text->set_overflowMode(TMPro::TextOverflowModes::Overflow);
    flyingScoreEffect->color = JudgeColor(total, before, after, accuracy, timeDependence, bestIndex.value());
}

std::string HSV::JudgeText(int score, int before, int after, int accuracy, float timeDependence, int bestIndex) {
    return DisplayModeFormat(score, before, after, accuracy, timeDependence, config.judgments[bestIndex]);
}

UnityEngine::Color HSV::JudgeColor(int score, int before, int after, int accuracy, float timeDependence, int bestIndex) {
    return config.judgments[bestIndex].color.value();
}

std::string HSV::JudgeSegment(int scoreForSegment, std::vector<Segment> judgments) {
    if(judgments.size() == 0) {
        std::string();
    }

    Segment bestSegment = getBestSegment(judgments, scoreForSegment).value();

    if(bestSegment.text.value().empty())
        return std::string();
    else
        return bestSegment.text.value();
}

std::string HSV::JudgeTimeSegment(int scoreForSegment, std::vector<TimeSegment> judgments) {
    if(judgments.size() == 0) {
        std::string();
    }

    TimeSegment bestSegment = getBestTimeSegment(judgments, scoreForSegment).value();

    if(bestSegment.text.value().empty())
        return std::string();
    else
        return bestSegment.text.value();
}

std::string HSV::ConvertTimeDependencePrecision(float timeDependence, int decimalOffset, int decimalPrecision) {
    float multiplier = std::pow(10, decimalOffset);
    std::stringstream stream;
    stream << std::fixed << std::setprecision(decimalPrecision) << (timeDependence * multiplier);
    std::string s = stream.str();
    return s;
}

std::string HSV::DisplayModeFormat(int score, int before, int after, int accuracy, float timeDependence, const Judgment& judgment) {
    auto formatString = judgment.text.value();
    std::string buildString;
    auto nextPercentIndex = formatString.find('%');
    if(nextPercentIndex == -1)
        return judgment.text.value();
    while(nextPercentIndex != -1) {
        buildString.append(formatString.substr(0, nextPercentIndex));
        if(formatString.length() == nextPercentIndex + 1) {
            formatString += " ";
        }

        auto specifier = formatString[nextPercentIndex + 1];

        switch (specifier) {
            case 'b':
                buildString.append(std::to_string(before));
                break;
            case 'c':
                buildString.append(std::to_string(accuracy));
                break;
            case 'a':
                buildString.append(std::to_string(after));
                break;
            case 't':
                buildString.append(ConvertTimeDependencePrecision(timeDependence, config.timeDependencyDecimalOffset, config.timeDependencyDecimalPrecision));
                break;
            case 'B':
                buildString.append(JudgeSegment(before, config.beforeCutAngleJudgments));
                break;
            case 'C':
                buildString.append(JudgeSegment(accuracy, config.accuracyJudgments));
                break;
            case 'A':
                buildString.append(JudgeSegment(after, config.afterCutAngleJudgments));
                break;
            case 'T':
                buildString.append(JudgeTimeSegment(timeDependence, config.timeDependencyJudgments));
                break;
            case 's':
                buildString.append(std::to_string(score));
                break;
            case 'p':
                buildString.append(string_format("%f", std::round(score / 115 * 100)));
                break;
            case '%':
                buildString.append("%");
                break;
            case '>':
                buildString.append("%>");
                break;
            case 'n':
                buildString.append("\n");
                break;
            default:
                buildString.append(("%" + to_string(specifier)));
                break;
        }

        formatString = formatString.erase(0, nextPercentIndex + 2);
        nextPercentIndex = formatString.find('%');
    }

    getLogger().info("%s", buildString.c_str());
    return buildString;
}

extern "C" void load() {
<<<<<<< Updated upstream
  il2cpp_functions::Init();
  QuestUI::Init();
  QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);
  HitScore::InstallHooks();
  INSTALL_HOOK(getLogger(), FlyingScoreEffectHook);
  custom_types::Register::AutoRegister();
=======
    il2cpp_functions::Init();
    custom_types::Register::AutoRegister();
    QuestUI::Init();
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);
    HSV::loadConfig();
    INSTALL_HOOK(getLogger(), InitFlyingScoreEffect);
    INSTALL_HOOK(getLogger(), FlyingScoreEffectHook);
    INSTALL_HOOK(getLogger(), HandleSwingFinish);
    INSTALL_HOOK(getLogger(), FlyingScoreEffectFinish);
    INSTALL_HOOK(getLogger(), FlyingScoreEffectManualUpdate);
>>>>>>> Stashed changes
}