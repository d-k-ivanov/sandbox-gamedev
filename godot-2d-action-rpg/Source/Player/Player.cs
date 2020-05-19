using Godot;
using System;

public class Player : KinematicBody2D
{
    [Export] public int MaxSpeed                = 160;
    [Export] public int RollSpeed               = 210;
    [Export] public int Acceleration            = 1000;
    [Export] public int Friction                = 1000;

    private Vector2                             _velocity;
    private Vector2                             _rollDirection;
    private Stats                               _stats;
    // private AnimationPlayer                  _animationPlayer;
    private AnimationTree                       _animationTree;
    private AnimationNodeStateMachinePlayback   _animationState;
    private SwordHitbox                         _swordHitbox;
    private Hurtbox                             _hurtbox;

    private enum PlayerState
    {
        Move,
        Roll,
        Attack
    }
    private PlayerState _playerState            = PlayerState.Move;

    public override void _Ready()
    {
        _velocity                   = Vector2.Zero;
        _rollDirection              = Vector2.Right;

        // Player stats
        _stats                      = GetNode("/root/PlayerStats") as Stats;
        _stats?.Connect("NoHealth", this, "Death");

        // _animationPlayer            = GetNode<AnimationPlayer>("AnimationPlayer");
        _animationTree              = GetNode<AnimationTree>("AnimationTree");
        _animationTree.Active       = true;
        _animationState             = (AnimationNodeStateMachinePlayback)_animationTree.Get("parameters/playback");
        _animationState.Start("Idle");

        _swordHitbox                = GetNode<SwordHitbox>("HitboxPivot/SwordHitbox");
        _swordHitbox.HitDirection   = Vector2.Right;

        _hurtbox                = GetNode<Hurtbox>("Hurtbox");

        GD.Print("Player is ready!");
    }


    public override void _PhysicsProcess(float delta)
    {
        switch (_playerState)
        {
            case PlayerState.Move:
                MoveState(delta);
                break;
            case PlayerState.Attack:
                AttackState();
                break;
            case PlayerState.Roll:
                RollState();
                break;
            default:
                throw new ArgumentOutOfRangeException();
        }
    }

    private void MoveState(float delta)
    {
        var motion = Vector2.Zero;
        motion.x = Input.GetActionStrength("ui_right") - Input.GetActionStrength("ui_left");
        motion.y = Input.GetActionStrength("ui_down")  - Input.GetActionStrength("ui_up");
        motion = motion.Normalized();

        if (motion != Vector2.Zero)
        {
            _rollDirection             = motion;
            _swordHitbox.HitDirection   = motion;

            _animationTree.Set("parameters/Idle/blend_position", motion);
            _animationTree.Set("parameters/Run/blend_position", motion);
            _animationTree.Set("parameters/Attack/blend_position", motion);
            _animationTree.Set("parameters/Roll/blend_position", motion);

            _animationState.Travel("Run");
            _velocity = _velocity.MoveToward(motion * MaxSpeed, Acceleration * delta);
        }
        else
        {
            _animationState.Travel("Idle");
            _velocity = _velocity.MoveToward(Vector2.Zero, Friction * delta) ;
        }

        Move();

        if (Input.IsActionJustPressed("roll"))
        {
            _playerState = PlayerState.Roll;
        }

        if (Input.IsActionJustPressed("attack"))
        {
            _playerState = PlayerState.Attack;
        }
    }
    private void AttackState()
    {
        _velocity = Vector2.Zero;
        _animationState.Travel("Attack");
    }

    private void RollState()
    {
        _velocity = _rollDirection * RollSpeed;
        _animationState.Travel("Roll");
        Move();
    }

    private void Move()
    {
        _velocity = MoveAndSlide( _velocity);
    }

    private void Death()
    {
        QueueFree();
    }

    private void OneTimeAnimationFinished()
    {
        _velocity *= (float) 0.7;
        _playerState = PlayerState.Move;
    }

    private void _on_Hurtbox_area_entered(object area)
    {
        _stats.Health -= 1;
        _hurtbox.StartInvincibility(0.5f);
        _hurtbox.CreateHitEffect();
        GD.Print($"{this.Name} health:\t{_stats.Health} of {_stats.MaxHealth}");
    }
}
