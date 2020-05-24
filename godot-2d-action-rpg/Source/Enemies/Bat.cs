using Godot;
using System;
using System.Collections.Generic;

public class Bat : KinematicBody2D
{
    [Export] public int MaxSpeed                = 100;
    [Export] public int Acceleration            = 400;
    [Export] public int Friction                = 800;

    private Vector2                             _velocity;
    private Vector2                             _knockBack;
    private AnimatedSprite                      _animatedSprite;
    private Stats                               _stats;
    private PlayerDetectionZone                 _playerDetectionZone;
    private Hurtbox                             _hurtbox;
    private SoftCollision                       _softCollision;
    private WanderController                    _wanderController;

    private readonly PackedScene _enemyDeathEffectScene = GD.Load("res://Source/Enemies/EnemyDeathEffect.tscn") as PackedScene;

    private enum BatState
    {
        Idle,
        Wander,
        Chase
    }
    private BatState _batState;

    public override void _Ready()
    {
        GD.Randomize();
        _velocity               = Vector2.Zero;
        _knockBack              = Vector2.Zero;
        _animatedSprite         = GetNode<AnimatedSprite>("AnimatedSprite");
        _stats                  = GetNode<Stats>("Stats");
        _playerDetectionZone    = GetNode<PlayerDetectionZone>("PlayerDetectionZone");
        _hurtbox                = GetNode<Hurtbox>("Hurtbox");
        _softCollision          = GetNode<SoftCollision>("SoftCollision");
        _wanderController       = GetNode<WanderController>("WanderController");
        _wanderController.StartWanderTimer((float) GD.RandRange(1,3));
        // GD.Print($"{this.Name} initial status:\tHealth={_stats.Health} MaxHealth={_stats.MaxHealth}");
        _batState               = PickRandomState(new List<BatState> {BatState.Idle, BatState.Wander});
    }

    public override void _PhysicsProcess(float delta)
    {
        _knockBack = _knockBack.MoveToward(Vector2.Zero, Friction * delta);
        _knockBack = MoveAndSlide(_knockBack );

        switch (_batState)
        {
            case BatState.Idle:
                _velocity = _velocity.MoveToward(Vector2.Zero, Friction * delta);
                Seek();
                break;
            case BatState.Wander:
                Seek();
                GoTowardPoint(_wanderController.TargetPosition, delta);
                if (GlobalPosition.DistanceTo(_wanderController.TargetPosition) <= _wanderController.WanderRange / 8f)
                {
                    _velocity = _velocity.MoveToward(Vector2.Zero, Friction * delta);
                }
                break;
            case BatState.Chase:
                var player = _playerDetectionZone.Player;
                if (player != null)
                {
                    GoTowardPoint(player.GlobalPosition, delta);
                }
                else
                {
                    _batState = BatState.Idle;
                }
                break;
            default:
                throw new ArgumentOutOfRangeException();
        }

        if (_softCollision.IsColliding())
        {
            _velocity += _softCollision.GetPushVector() * delta * 400;
        }
        _velocity = MoveAndSlide(_velocity);
    }

    private void GoTowardPoint(Vector2 point, float delta)
    {
        var direction = GlobalPosition.DirectionTo(point);
        _velocity = _velocity.MoveToward(direction * MaxSpeed, Acceleration * delta);
        _animatedSprite.FlipH = _velocity.x < 0;
    }

    private void Seek()
    {
        if (_wanderController.GetTimeLeft() == 0)
        {
            GD.Print(_wanderController.GetTimeLeft());
            _batState = PickRandomState(new List<BatState> {BatState.Idle, BatState.Wander});
            _wanderController.StartWanderTimer((float) GD.RandRange(1,3));
        }
        if (_playerDetectionZone.IsPlayerDetected())
        {
            _batState = BatState.Chase;
        }
    }

    private static BatState PickRandomState(IList<BatState> stateList)
    {
        // Fisher-Yates shuffle
        var random                 = new Random((int) GD.RandRange(0f, 65535f));
        var stateListCount         = stateList.Count;
        while (stateListCount > 1)
        {
            stateListCount--;
            var nextIndex             = random.Next(stateListCount + 1);
            var tempValue             = stateList[nextIndex];
            stateList[nextIndex]      = stateList[stateListCount];
            stateList[stateListCount] = tempValue;
        }
        GD.Print(stateList[0]);
        return stateList[0];
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
        _knockBack = area.HitDirection * 250;
        _hurtbox.CreateHitEffect();
    }

    private void CreateDeathEffect()
    {
        var deathEffectNode = _enemyDeathEffectScene.Instance() as Effect;
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

