# HitScoreVisualizer

A fairly simple mod that allows you to heavily customize the appearance of hit scores.

## Usage

After installing the mod, if your config files have the extension `.hsv` or `.hsvconfig`, you can upload them directly to MBF. Otherwise, they can be manually placed in the folder `/sdcard/ModData/com.beatgames.beatsaber/Mods/HitScoreVisualizer`, or even subfolders if desired, regardless of file name or extension. Note that the folder may not exist yet if you have not launched the game again after installing the mod.

> [!TIP]
> While you can have config files with the same name when using folder structures, **I still strongly advise you to use unique config filenames** because, despite HSV being able to handle them just fine, you might end up with several files in the list that appear identical.

After uploading your configs, you can open the settings ingame and select one.

## Creating a Custom Config

There are various places that configs can be sourced, and there's nothing wrong with using the default config or copying someone else's file and not worrying about it. However, if you want to use the full customizability of the mod, this is how to create and modify a config file.

> [!NOTE]
> The default config, used when you have no others selected, is no longer an actual file created by the mod due to issues with people not setting `isDefaultConfig` to `false` when sharing their configs. It can still be referenced as a starting point, and can be found in the [default_config.json](default_config.json) in this repository.

Following is a list of all the properties in the config used by the current version of the mod. Extra values may be found in older configs or those sourced from the PC version, but any property not in this table will simply be ignored.

| Property name(s) | Explanation / Info | Example or possible values |
| --- | --- | --- |
| `fixedPosition` | Optional coordinates of a fixed location where the hit scores will be shown.<br/>Either leave this out or set as `null` to use normal hit score positions and animation. | <ul><li>```{"x": 0.0, "y": 3.0, "z": -2.0}```</li><li>`null`</li></ul> |
| `targetPositionOffset` | An optional vector that offsets the destination location of every hit score.<br/>If a fixed position is defined, it will take priority and this will be ignored. | <ul><li>```{"x": 0.0, "y": 3.0, "z": -2.0}```</li><li>`null`</li></ul> |
| `judgments` | A list of Judgments used to customize the general Judgments for non-chain hit scores. | Uses Judgment objects.<br/>More info below. |
| `beforeCutAngleJudgments` | A list used to customize the Judgments for the part of the swing before cutting the block (0 - 70).<br/>Format token: `%B` | Uses JudgmentSegments objects.<br/>More info below. |
| `afterCutAngleJudgments` | A list used to customize the Judgments for the part of the swing after cutting the block (0 - 30).<br/>Format token: `%A`<br/> | Uses JudgmentSegments.<br/>More info below. |
| `accuracyJudgments` | A list used to customize the Judgments for the accuracy of the cut (0 - 15).<br/>Format token: `%C`<br/> | Uses JudgmentSegments objects.<br/>More info below. |
| `timeDependencyJudgments` | A list used to customize the Judgments for the time dependence (0 - 1).<br/>Format token: `%T`<br/> | Uses TimeDependenceJudgmentSegments.<br/>More info below. |
| `chainHeadJudgments` | A list used to customize the Judgments for chain head hit scores. | Uses Judgment objects. More info below. |
| `chainLinkDisplay` | A Judgment that will always display when hitting the links of the chain block.<br/>Some fields and tokens don't do anything here because the links are always the same score. | Uses Judgment objects. More info below. |
| `timeDependencyDecimalPrecision` | The number of decimal places used when time dependence is shown.<br/>**Must be between 0 and 99, inclusive** | <ul><li>`0`</li><li>`5`</li><li>`80`</li></ul> |
| `timeDependencyDecimalOffset` | A power of 10 that the displayed time dependence will be multiplied by.<br/>Time dependence is from 0 - 1, so this will allow it to be shown between 0 and 10, 0 and 100, etc.<br/> **Must be between 0 and 38, inclusive** | <ul><li>`0`</li><li>`5`</li><li>`38`</li></ul> |
| `badCutDisplays` | A list of BadCutDisplays that can change the regular bad cut text. | Uses BadCutDisplay objects. More info below. |
| `randomizeBadCutDisplays` | If true, a random item from the `badCutDisplays` list will be shown for every bad cut. Otherwise, it will go through them in order. | `true` |
| `missDisplays` | A list of MissDisplays that can change the regular miss text. | Uses MissDisplay objects. More info below. |
| `randomizeMissDisplays` | If true, a random item from the `missDisplays` list will be shown for every miss. Otherwise, it will go through them in order. | `true` |

### Important info

- The `text` property of Judgment, JudgmentSegment, TimeDependenceJudgmentSegment, BadCutDisplay, and MissDisplay objects all have support for [TextMeshPro formatting](http://digitalnativestudios.com/textmeshpro/docs/rich-text/).
- The order of Judgments and JudgmentSegments in their lists does not matter, unless none of the Judgments fit the threshold for a hit score, in which case the last one will be used.
- `chainHeadJudgments` and `chainLinkDisplay` are not required in configs for backwards compatibility, and configs without them will simply not affect the displayed scores for those types of notes.
- Bad cut and miss related fields are also not required in configs.

### Format tokens

| Token | Explanation / Info |
| --- | --- |
| `%b` | The score contributed by the swing before cutting the block. |
| `%a` | The score contributed by the part of the swing after cutting the block. |
| `%c` | The score contributed by the accuracy of the cut. |
| `%t` | The time dependence of the swing. This value indicates how depedent the accuracy part of the score is upon *when* you hit the block, measured from 0 - 1. A value of 0 indicates a completely time independent swing, while a value of 1 indicates that the accuracy part of the score would vary greatly if the block was hit even slightly earlier or later.
| `%B`, `%C`, `%A`, and `%T` | Uses the Judgment text that matches the threshold as specified in either `beforeCutAngleJudgments`, `accuracyJudgments`, `afterCutAngleJudgments`, or `timeDependencyJudgments` (depending on the used token). |
| `%s` | The total score of the cut. |
| `%p` | The percentage of the total score of the cut out of the maximum possible. |
| `%d` | An arrow pointing in the direction from the cut plane to the center of the note. (So if the note is cut too far on its right side, the arrow will point to the left.) |
| `%%` | A literal percent symbol. |
| `%n` | A newline. |

### Judgment objects

| Property name(s) | Explanation / Info | Example or possible values |
| --- | --- | --- |
| `threshold` | The threshold that defines if this Judgment will be used for a given score. The Judgment will be used if it is the one with the highest threshold that's either equal or smaller than the given hit score.<br/>It can also be omitted when it's the Judgment for the lowest scores. | `110` |
| `text` | The text to display. This can contain the formatting tokens seen above, and they will be replaced with their corresponding values or segments. | `"%BFantastic%A%n%s"` |
| `color` | An array that specifies the color. Consists of 4 floating numbers ranging between (inclusive) 0 and 1, corresponding to Red, Green, Blue, and Alpha. | `[0, 0.5, 1, 0.75]` |
| `fade` | If true, the text color will be interpolated between this Judgment's color and the Judgment with the next highest threshold based on how close to the next threshold it is. | `false` |

### JudgmentSegment objects

| Property name(s) | Explanation / Info | Example or possible values |
| --- | --- | --- |
| `threshold` | The threshold that defines if this segment will be used for a given score. The segment will be used if it is the one with the highest threshold that's either equal or smaller than the given part of the hit score.<br/>It can also be omitted for the segment for the lowest scores. | `30` |
| `text` | The text to display. The above format tokens will not be replaced in this text. | `"+++"` |

### TimeDependenceJudgmentSegment objects

| Property name(s) | Explanation / Info | Example or possible values |
| --- | --- | --- |
| `threshold` | The threshold that defines if this segment will be used for a given time dependence. The segment will be used if it is the one with the highest threshold that's either equal or smaller than the time dependence.<br/>It can also be omitted for the segment for the lowest time dependences. | `0.5` |
| `text` | The text to display. The above format tokens will not be replaced in this text. | `"+++"` |

### BadCutDisplay objects

| Property name(s) | Explanation / Info | Example or possible values |
| --- | --- | --- |
| `text` | The text to display. No format tokens will be replaced. | `"Oops"` |
| `type` | The type of bad cuts this text can be shown for. If omitted, it can be shown for any bad cut. | <ul><li>`"WrongDirection"`</li><li>`"WrongColor"`</li><li>`"Bomb"`</li><li>`"All"`</li></ul> |
| `color` | An array that specifies the color. Consists of 4 floating numbers ranging between (inclusive) 0 and 1, corresponding to Red, Green, Blue, and Alpha. | `[0, 0.5, 1, 0.75]` |

### MissDisplay objects

| Property name(s) | Explanation / Info | Example or possible values |
| --- | --- | --- |
| `text` | The text to display. No format tokens will be replaced. | `"Oops 2"` |
| `color` | An array that specifies the color. Consists of 4 floating numbers ranging between (inclusive) 0 and 1, corresponding to Red, Green, Blue, and Alpha. | `[0, 0.5, 1, 0.75]` |

## Useful links

[HSV Preview by Isaiah Billingsley](https://hsv-preview.netlify.app/): A website that allows you to edit an HSV config file with a preview.

[HSV Config Creator by MoreOwO](https://github.com/MoreOwO/HSV-Config-Creator/releases/latest): A program that helps you create configs for HSV. (Out of date for some features.)

## Credits

- [artemiswkearney](https://github.com/artemiswkearney), [ErisApps](https://github.com/ErisApps), and [qqrz](https://github.com/qqrz997) for the PC mod
- [Metalit](https://github.com/Metalit) for maintaining the quest mod now
