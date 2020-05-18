using Godot;
using System;

public class Bat : KinematicBody2D
{
    [Export] public int MaxSpeed                = 100;
    [Export] public int Acceleration            = 200;
    [Export] public int Friction                = 200;

    private Vector2 _velocity;
    private Vector2 _knockBack;
    private AnimatedSprite _animatedSprite;
    private Stats _stats;
    private PlayerDetectionZone _playerDetectionZone;

    private readonly PackedScene _enemyDeathEffectScene = GD.Load("res://Source/Enemies/EnemyDeathEffect.tscn") as PackedScene;

    private enum BatState
    {
        Idle,
        Wander,
        Chase
    }
    private BatState _batState            = BatState.Idle;

    public override void _Ready()
    {
        _velocity            = Vector2.Zero;
        _knockBack           = Vector2.Zero;
        _animatedSprite      = GetNode<AnimatedSprite>("AnimatedSprite");
        _stats               = GetNode<Stats>("Stats");
        _playerDetectionZone = GetNode<PlayerDetectionZone>("PlayerDetectionZone");
        // GD.Print($"{this.Name} initial status:\tHealth={_stats.Health} MaxHealth={_stats.MaxHealth}");
    }

    public override void _PhysicsProcess(float delta)
    {
        _knockBack = _knockBack.MoveToward(Vector2.Zero, Friction * delta);
        _knockBack = MoveAndSlide(_knockBack );

        switch (_batState)
        {
            case BatState.Idle:
                _velocity = _velocity.MoveToward(Vector2.Zero, Friction * delta);
                SeekPlayer();
                break;
            case BatState.Wander:
                break;
            case BatState.Chase:
                var player = _playerDetectionZone.Player;
                if (player != null)
                {
                    var direction = (player.GlobalPosition - GlobalPosition).Normalized();
                    _velocity = _velocity.MoveToward(direction * MaxSpeed, Acceleration * delta);
                }
                else
                {
                    _batState = BatState.Idle;
                }
                break;
            default:
                throw new ArgumentOutOfRangeException();
        }

        _animatedSprite.FlipH = _velocity.x < 0;
        _velocity = MoveAndSlide(_velocity);
    }

    private void SeekPlayer()
    {
        if (_playerDetectionZone.IsPlayerDetected())
        {
            _batState = BatState.Chase;
        }
    }

    private void _on_Hurtbox_area_entered(SwordHitbox area)
    {
        // _stats.Health -= 1;
        _stats.Health -= area.Damage;

        // Kill v.1
        // if (_stats.Health <= 0)
        // {
        //     QueueFree();
        //
        // }

        GD.Print($"{this.Name} health:\t{_stats.Health} of {_stats.MaxHealth}");
        _knockBack = area.HitDirection * 120;
    }

    private void CreateDeathEffect()
    {
        var deathEffectNode           = _enemyDeathEffectScene.Instance() as Effect;
        GetParent().AddChild(deathEffectNode);
        if (deathEffectNode != null) deathEffectNode.GlobalPosition = GlobalPosition;
    }

    // Kill v.2 - Signals
    private void _on_Stats_NoHealth()
    {
        QueueFree();
        CreateDeathEffect();
    }

}

