using Godot;
using System;

public class Player : KinematicBody2D
{
    [Export] public int MaxSpeed       = 200;
    [Export] public int Acceleration   = 100;
    private const   int SpeedFraction  = 100;

    private Vector2     _velocity      = Vector2.Zero;

    public override void _Ready()
    {
        GD.Print("Player.cs is ready!");
    }

    public override void _PhysicsProcess(float delta)
    {
        var motion = Vector2.Zero;
        motion.x = Input.GetActionStrength("ui_right") - Input.GetActionStrength("ui_left");
        motion.y = Input.GetActionStrength("ui_down")  - Input.GetActionStrength("ui_up");
        motion = motion.Normalized();

        if (motion != Vector2.Zero)
        {
            _velocity += motion * Acceleration * delta;
            _velocity = _velocity.Clamped(MaxSpeed * delta);
        }
        else
        {
            _velocity = _velocity.MoveToward(Vector2.Zero, SpeedFraction * delta) ;
        }

        GD.Print(_velocity);

        MoveAndCollide( _velocity);
    }
}
