using Godot;
using System;

public class Player : KinematicBody2D
{
    [Export]
    public int speed = 200;

    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        GD.Print("Player.cs is ready!");
    }

    // Called every frame. 'delta' is the elapsed time since the previous frame.
    public override void _PhysicsProcess(float delta)
    {
        var motion = new Vector2();
        motion.x = Input.GetActionStrength("ui_right") - Input.GetActionStrength("ui_left");
        motion.y = Input.GetActionStrength("ui_down") - Input.GetActionStrength("ui_up");

        MoveAndCollide(motion.Normalized() * speed * delta);
    }
}
