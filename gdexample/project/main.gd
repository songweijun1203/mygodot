extends Node2D

func _ready() -> void:
	var example := GdExample.new()
	example.name = "GdExample"
	example.position = Vector2(320, 180)
	example.message = "Created at runtime"
	add_child(example)
