using Godot;
using System;

public class Player : KinematicBody2D
{
    [Export] public int MaxSpeed            = 160;
    [Export] public int Acceleration        = 1000;
    private  const  int BreakingFraction    = 1000;

    private Vector2 _velocity = Vector2.Zero;

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
            _velocity = _velocity.MoveToward(motion * MaxSpeed, Acceleration * delta);
        }
        else
        {
            _velocity = _velocity.MoveToward(Vector2.Zero, BreakingFraction * delta) ;
        }

        // GD.Print(_velocity);

        MoveAndSlide( _velocity);
    }
}
