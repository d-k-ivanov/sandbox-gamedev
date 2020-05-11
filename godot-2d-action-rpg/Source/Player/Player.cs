using Godot;
using System;

public class Player : KinematicBody2D
{
    [Export] public int MaxSpeed            = 160;
    [Export] public int Acceleration        = 1000;
    private  const  int BreakingFraction    = 1000;

    private Vector2 _velocity               = Vector2.Zero;

    private AnimationPlayer                     _animationPlayer;
    private AnimationTree                       _animationTree;
    private AnimationNodeStateMachinePlayback   _animationState;

    public override void _Ready()
    {
        _animationPlayer        = GetNode<AnimationPlayer>("AnimationPlayer");
        _animationTree          = GetNode<AnimationTree>("AnimationTree");
        _animationTree.Active   = true;
        _animationState         = (AnimationNodeStateMachinePlayback)_animationTree.Get("parameters/playback");
        _animationState.Start("Idle");

        GD.Print("Player is ready!");
    }


    public override void _PhysicsProcess(float delta)
    {
        var motion = Vector2.Zero;
        motion.x = Input.GetActionStrength("ui_right") - Input.GetActionStrength("ui_left");
        motion.y = Input.GetActionStrength("ui_down")  - Input.GetActionStrength("ui_up");
        motion = motion.Normalized();

        if (motion != Vector2.Zero)
        {
            // _animationPlayer.Play(motion.x > 0 ? "RunRight" : "RunLeft");
            _animationTree.Set("parameters/Idle/blend_position", motion);
            _animationTree.Set("parameters/Run/blend_position", motion);
            _animationState.Travel("Run");
            _velocity = _velocity.MoveToward(motion * MaxSpeed, Acceleration * delta);
        }
        else
        {
            // _animationPlayer.Play("IdleRight");
            _animationState.Travel("Idle");
            _velocity = _velocity.MoveToward(Vector2.Zero, BreakingFraction * delta) ;
        }

        // GD.Print(_velocity);

        MoveAndSlide( _velocity);
    }
}
