extends Node2D

@onready var player = $Player
@onready var target = $Target
@onready var player_area = $Player/Area2D
@onready var player_animation = $Player/RenderRoot/Anim2D
@onready var status = $HUD/Status

var trigger_enters := 0
var trigger_exits := 0
var body_enters := 0


func _ready() -> void:
	player_area.area_entered.connect(_on_player_area_entered)
	player_area.area_exited.connect(_on_player_area_exited)
	player_area.body_entered.connect(_on_player_body_entered)
	call_deferred("_start_probe")


func _start_probe() -> void:
	# The native SpxSprite physics loop consumes this initial velocity.
	player.velocity = Vector2(170, 0)
	player_animation.play("default")
	_update_status()


func _process(_delta: float) -> void:
	_update_status()


func _unhandled_input(event: InputEvent) -> void:
	if event.is_action_pressed("ui_accept"):
		player.global_position = Vector2(250, 120)
		player.velocity = Vector2(170, 0)
		trigger_enters = 0
		trigger_exits = 0
		body_enters = 0


func _on_player_area_entered(_area: Area2D) -> void:
	trigger_enters += 1


func _on_player_area_exited(_area: Area2D) -> void:
	trigger_exits += 1


func _on_player_body_entered(_body: Node2D) -> void:
	body_enters += 1


func _update_status() -> void:
	if not is_instance_valid(status) or not is_instance_valid(player):
		return

	status.text = "Player gid: %s   Target gid: %s\nPhysics mode: %s (2 = Dynamic)\nPosition: %s\nOn floor: %s\nAnimation playing: %s\nTrigger enters/exits: %d / %d\nBody enters: %d" % [
		player.gid,
		target.gid,
		player.physics_mode,
		player.global_position.round(),
		player.is_on_floor(),
		player_animation.is_playing(),
		trigger_enters,
		trigger_exits,
		body_enters,
	]
