/**************************************************************************/
/*  spx_audio_mgr.h                                                       */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef SPX_AUDIO_MGR_H
#define SPX_AUDIO_MGR_H

#include "core/templates/hash_map.h"
#include "gdextension_spx_ext.h"
#include "scene/2d/node_2d.h"
#include "scene/main/node.h"
#include "spx_audio.h"
#include "spx_object_mgr.h"

class SpxAudioMgr : public SpxObjectMgr<SpxAudio> {

private:
	// Main-thread-only routing index. Player state lives solely in SpxAudio.
	HashMap<GdInt, GdObj> aid_owners;
	GdInt g_audio_id = 0;

	SpxAudio *_get_aid_audio(GdInt aid);

public:

	void on_awake() override;
	void on_destroy() override;
	void on_update(float delta) override;
	void on_reset(int reset_code) override;

	// 术语约定：
	// obj = 音频组 ID（create_audio 返回）。组是逻辑容器，持有总线与默认属性
	//       （pitch/volume/pan），不是场景节点。
	// aid = 播放 ID（play* 返回，全局自增，0 = 失败/播放已被回收）。一次播放对应
	//       一个临时 AudioStreamPlayer2D 节点，控制类接口按 aid 经 aid_owners 路由寻址。
	// 所有接口只允许在引擎主线程调用（非主线程调用会被拒绝并打印 ERR）。

	// 停止所有组的全部播放并清空 aid 路由表。
	SPX_BIND void stop_all();
	// 新建一个音频组，返回组 ID obj。
	SPX_BIND GdObj create_audio();
	// 销毁音频组 obj：组内所有播放立即停止并回收，该组名下所有 aid 随之失效。
	SPX_BIND void destroy_audio(GdObj obj);

	// 组默认音高倍率（1.0=原速，2.0=快一倍且升调）。
	// 只对之后的 play 生效，不改变正在播放的 voice。
	SPX_BIND void set_pitch(GdObj obj, GdFloat pitch);
	SPX_BIND GdFloat get_pitch(GdObj obj);
	// 组声像：-1 全左、0 居中、1 全右（实现为总线挂 AudioEffectPanner）。
	// 首次调用会触发 ensure_dedicated_bus()：该组从共享的 Sfx 总线迁移到一条
	// 专属池化总线，此后组内所有播放共享此声像/音量。
	SPX_BIND void set_pan(GdObj obj, GdFloat pan);
	SPX_BIND GdFloat get_pan(GdObj obj);
	// 组音量：线性 0..1（内部换算为 dB，0=静音）。作用域与总线迁移同 set_pan。
	SPX_BIND void set_volume(GdObj obj, GdFloat volume);
	SPX_BIND GdFloat get_volume(GdObj obj);

	// 在组 obj 里播放 path（res:// 等加载器可识别的音频路径），返回播放 ID aid。
	// owner_id 决定本次播放的声源节点挂在哪个场景节点下（声源站在哪，就对谁衰减）：
	//   -1          当前相机。声源每帧与听者重合（dist≡0）→ 固定音量+居中声像，BGM 用这个；
	//   精灵 ID     声源跟随该精灵，产生位置感（距离衰减+左右声像），适合音效；
	//   其它/查不到  audio_root 世界原点 (0,0)。相机的移动会拉开声源与听者的距离。
	// 听者（耳朵）= 视口注册的 AudioListener2D；不存在时 = 屏幕中心对应的世界坐标
	//（即当前相机的实际渲染位置）。
	// 衰减遵循 Godot AudioStreamPlayer2D 规则：
	//   dist > max_distance         该路声音整帧跳过，完全静音（不是变小）；
	//   否则 音量乘数 = (1 - dist/max_distance) ^ attenuation。
	// attenuation 越大衰减越陡（默认 1.0）；max_distance 单位是像素（默认 2000）。
	SPX_BIND GdInt play_with_attenuation(GdObj obj, GdString path, GdObj owner_id, GdFloat attenuation, GdFloat max_distance);
	// 便捷入口：等价于 play_with_attenuation(obj, path, owner_id=0, ...)。
	// FIXME: 实现里实参 (2000, 1) 按参数顺序落在 attenuation=2000、max_distance=1，
	// 与 SpxAudio::play 的默认值 (1.0, 2000.0) 恰好对调 —— 相机离开世界原点
	// 1px 该声源即被整体静音。修复前 BGM/非定位声请勿用本接口，改走
	// play_with_attenuation(owner_id=-1)。
	SPX_BIND GdInt play(GdObj obj, GdString path);
	// 暂停 aid 的播放：节点保留，resume 可从暂停处续播。
	SPX_BIND void pause(GdInt aid);
	// 从暂停处恢复播放。
	SPX_BIND void resume(GdInt aid);
	// 立即停止并销毁本次播放（stop+queue_free），循环随之终止。
	// 同一音频想再播必须重新 play 获取新 aid。
	SPX_BIND void stop(GdInt aid);
	// 从头重播（seek 0 + play）。voice 已被回收时返回 false，需重新 play。
	SPX_BIND GdBool restart(GdInt aid);
	// 标记循环。实现是轮询式：SpxAudio::on_update 检测到播完后重新 trigger play，
	// 衔接处有无至多一帧的间隙。必须在 play 之后对有效 aid 调用；
	// aid 无效时静默失败（表现为“设置了但没循环”）。
	SPX_BIND void set_loop(GdInt aid, GdBool loop);
	SPX_BIND GdBool get_loop(GdInt aid);

	// 播放进度，单位秒（读 AudioStreamPlayer2D::get_playback_position，
	// 推进速度受 pitch 影响）。voice 不存在时返回 0。
	SPX_BIND GdFloat get_timer(GdInt aid);
	// 跳转播放进度（seek），单位秒。
	SPX_BIND void set_timer(GdInt aid, GdFloat time);
	// 是否占用播放槽：正在播放、或处于暂停态都返回 true。
	SPX_BIND GdBool is_playing(GdInt aid);
};

#endif // SPX_AUDIO_MGR_H
