#pragma once

#include "TokenizedText.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Vector3.hpp"
#include "config-utils/shared/config-utils.hpp"

namespace HSV {
    inline std::vector<std::string> const BadCutTypes = {"All", "WrongDirection", "WrongColor", "Bomb"};

    DECLARE_JSON_STRUCT(ColorArray) {
        NAMED_VECTOR(float, RawColor, SELF_OBJECT_NAME);
        DESERIALIZE_FUNCTION(ParseColor) {
            if (RawColor.size() != 4)
                throw JSONException("invalid color array length");
            Color = {RawColor[0], RawColor[1], RawColor[2], RawColor[3]};
        };
        UnityEngine::Color Color;

        ColorArray(UnityEngine::Color color) : RawColor{color.r, color.g, color.b, color.a}, Color(color) {}
        ColorArray() = default;
    };

    DECLARE_JSON_STRUCT(Judgement) {
       private:
        NAMED_VALUE(std::string, UnprocessedText, "text");
        DESERIALIZE_FUNCTION(ParseText) {
            Text = TokenizedText(UnprocessedText);
        };

       public:
        NAMED_VALUE(ColorArray, Color, "color");
        NAMED_VALUE_DEFAULT(int, Threshold, 0, "threshold");
        NAMED_VALUE_OPTIONAL(bool, Fade, "fade");
        TokenizedText Text;

        Judgement(int threshold, std::string text, UnityEngine::Color color, bool fade = false) :
            Threshold(threshold),
            Text(text),
            Color(color),
            Fade(fade) {}
        Judgement() = default;
    };

    DECLARE_JSON_STRUCT(Segment) {
        NAMED_VALUE_DEFAULT(int, Threshold, 0, "threshold");
        NAMED_VALUE(std::string, Text, "text");

        Segment(int threshold, std::string text) : Threshold(threshold), Text(text) {}
        Segment() = default;
    };

    DECLARE_JSON_STRUCT(FloatSegment) {
        NAMED_VALUE_DEFAULT(float, Threshold, 0, "threshold");
        NAMED_VALUE(std::string, Text, "text");

        FloatSegment(int threshold, std::string text) : Threshold(threshold), Text(text) {}
        FloatSegment() = default;
    };

    DECLARE_JSON_STRUCT(BadCutDisplay) {
        NAMED_VALUE(std::string, Text, "text");
        NAMED_VALUE_DEFAULT(std::string, Type, "type", BadCutTypes[0]);
        NAMED_VALUE(ColorArray, Color, "color");

        DESERIALIZE_FUNCTION(ValidateType) {
            if (std::find(BadCutTypes.begin(), BadCutTypes.end(), Type) == BadCutTypes.end())
                throw JSONException("invalid display type");
        }
    };

    DECLARE_JSON_STRUCT(MissDisplay) {
        NAMED_VALUE(std::string, Text, "text");
        NAMED_VALUE(ColorArray, Color, "color");
    };

    DECLARE_JSON_STRUCT(Config) {
        NAMED_VECTOR(Judgement, Judgements, "judgments");
        DESERIALIZE_FUNCTION(ValidateJudgements) {
            if (Judgements.size() < 1)
                throw JSONException("no judgements found in config");
        };
        NAMED_VECTOR_DEFAULT(Judgement, ChainHeadJudgements, {}, "chainHeadJudgments");
        NAMED_VALUE_OPTIONAL(Judgement, ChainLinkDisplay, "chainLinkDisplay");
        NAMED_VECTOR_DEFAULT(Segment, BeforeCutAngleSegments, {}, "beforeCutAngleJudgments");
        NAMED_VECTOR_DEFAULT(Segment, AccuracySegments, {}, "accuracyJudgments");
        NAMED_VECTOR_DEFAULT(Segment, AfterCutAngleSegments, {}, "afterCutAngleJudgments");
        NAMED_VECTOR_DEFAULT(FloatSegment, TimeDependenceSegments, {}, "timeDependencyJudgments");
        NAMED_VALUE_OPTIONAL(float, FixedPosX, "fixedPosX");
        NAMED_VALUE_OPTIONAL(float, FixedPosY, "fixedPosY");
        NAMED_VALUE_OPTIONAL(float, FixedPosZ, "fixedPosZ");
        NAMED_VALUE_OPTIONAL(bool, UseFixedPos, "useFixedPos");
        NAMED_VALUE_OPTIONAL(ConfigUtils::Vector3, UnprocessedFixedPos, "fixedPosition");
        NAMED_VALUE_OPTIONAL(ConfigUtils::Vector3, UnprocessedPosOffset, "targetPositionOffset");
        NAMED_VALUE_DEFAULT(int, TimeDependenceDecimalPrecision, 1, "timeDependencyDecimalPrecision");
        NAMED_VALUE_DEFAULT(int, TimeDependenceDecimalOffset, 2, "timeDependencyDecimalOffset");
        NAMED_VECTOR_DEFAULT(BadCutDisplay, BadCutDisplays, {}, "badCutDisplays");
        NAMED_VALUE_DEFAULT(bool, RandomizeBadCutDisplays, true, "randomizeBadCutDisplays");
        NAMED_VECTOR_DEFAULT(MissDisplay, MissDisplays, {}, "missDisplays");
        NAMED_VALUE_DEFAULT(bool, RandomizeMissDisplays, true, "randomizeMissDisplays");

        std::optional<ConfigUtils::Vector3> FixedPos;
        std::optional<ConfigUtils::Vector3> PosOffset;

        std::vector<BadCutDisplay> WrongDirections;
        std::vector<BadCutDisplay> WrongColors;
        std::vector<BadCutDisplay> Bombs;

        DESERIALIZE_FUNCTION(ConvertPositions) {
            if (UseFixedPos.has_value() && UseFixedPos.value())
                FixedPos = {FixedPosX.value_or(0), FixedPosY.value_or(0), FixedPosZ.value_or(0)};
            else if (UnprocessedFixedPos.has_value())
                FixedPos = {UnprocessedFixedPos->x, UnprocessedFixedPos->y, UnprocessedFixedPos->z};
            if (UnprocessedPosOffset)
                PosOffset = {UnprocessedPosOffset->x, UnprocessedPosOffset->y, UnprocessedPosOffset->z};
        };

        DESERIALIZE_FUNCTION(CategorizeBadCuts) {
            WrongDirections.clear();
            WrongColors.clear();
            Bombs.clear();
            for (auto& display : BadCutDisplays) {
                if (display.Type == BadCutTypes[0] || display.Type == BadCutTypes[1])
                    WrongDirections.emplace_back(display);
                if (display.Type == BadCutTypes[0] || display.Type == BadCutTypes[2])
                    WrongColors.emplace_back(display);
                if (display.Type == BadCutTypes[0] || display.Type == BadCutTypes[3])
                    Bombs.emplace_back(display);
            }
        };

        bool HasChainHead() {
            return ChainHeadJudgements.size() > 0;
        };
        bool HasChainLink() {
            return ChainLinkDisplay.has_value();
        };
    };
}
