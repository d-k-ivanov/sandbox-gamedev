using Godot;
using System;
using System.Collections.Generic;

public partial class Bat : CharacterBody2D
{
    [Export] public int MaxSpeed                = 100;
    [Export] public int Acceleration            = 400;
    [Export] public int Friction                = 400;

    private Vector2                             _velocity;
    private Vector2                             _knockBack;
    private AnimatedSprite2D                    _animatedSprite;
    private Stats                               _stats;
    private PlayerDetectionZone                 _playerDetectionZone;
    private Hurtbox                             _hurtbox;
    private SoftCollision                       _softCollision;
    private WanderController                    _wanderController;
    private AnimationPlayer                     _animationPlayer;

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
        _animatedSprite         = GetNode<AnimatedSprite2D>("AnimatedSprite2D");
        _stats                  = GetNode<Stats>("Stats");
        _playerDetectionZone    = GetNode<PlayerDetectionZone>("PlayerDetectionZone");
        _hurtbox                = GetNode<Hurtbox>("Hurtbox");
        _softCollision          = GetNode<SoftCollision>("SoftCollision");
        _wanderController       = GetNode<WanderController>("WanderController");
        _animationPlayer        = GetNode<AnimationPlayer>("AnimationPlayer");
        _wanderController.StartWanderTimer(GD.RandRange(1,3));
        // GD.Print($"{this.Name} initial status:\tHealth={_stats.Health} MaxHealth={_stats.MaxHealth}");
        _batState               = PickRandomState(new List<BatState> {BatState.Idle, BatState.Wander});
    }

    public override void _PhysicsProcess(double delta)
    {
        Velocity = _knockBack.MoveToward(Vector2.Zero, Friction * (float) delta);
        MoveAndSlide();
        _knockBack = Velocity;

        switch (_batState)
        {
            case BatState.Idle:
                _velocity = _velocity.MoveToward(Vector2.Zero, Friction * (float) delta);
                Seek();
                break;
            case BatState.Wander:
                Seek();
                GoTowardPoint(_wanderController.TargetPosition, (float) delta);
                if (GlobalPosition.DistanceTo(_wanderController.TargetPosition) <= _wanderController.WanderRange / 8f)
                {
                    _velocity = _velocity.MoveToward(Vector2.Zero, Friction * (float) delta);
                }
                break;
            case BatState.Chase:
                var player = _playerDetectionZone.Player;
                if (player != null)
                {
                    GoTowardPoint(player.GlobalPosition, (float) delta);
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
            _velocity += _softCollision.GetPushVector() * (float) delta * 400;
        }

        Velocity = _velocity;
        MoveAndSlide();
        _velocity = Velocity;
    }

    private void GoTowardPoint(Vector2 point, float delta)
    {
        var direction = GlobalPosition.DirectionTo(point);
        _velocity = _velocity.MoveToward(direction * MaxSpeed, Acceleration * delta);
        _animatedSprite.FlipH = _velocity.X < 0;
    }

    private void Seek()
    {
        if (_wanderController.GetTimeLeft() == 0)
        {
            _batState = PickRandomState(new List<BatState> {BatState.Idle, BatState.Wander});
            _wanderController.StartWanderTimer(GD.RandRange(1,3));
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
            var nextIndex = random.Next(stateListCount + 1);
            (stateList[nextIndex], stateList[stateListCount]) = (stateList[stateListCount], stateList[nextIndex]);
        }
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
        _knockBack = area.HitDirection * 220;
        _hurtbox.CreateHitEffect();
        _hurtbox.StartInvincibility(0.4f);
    }

    private void CreateDeathEffect()
    {
        var deathEffectNode = _enemyDeathEffectScene.Instantiate() as Effect;
        GetParent().AddChild(deathEffectNode);
        if (deathEffectNode != null) deathEffectNode.GlobalPosition = GlobalPosition;
    }

    // Kill v.2 - Signals
    private void _on_Stats_NoHealth()
    {
        QueueFree();
        CreateDeathEffect();
    }

    private void _on_Hurtbox_InvincibilityStarted()
    {
        _animationPlayer.Play("Start");
    }

    private void _on_Hurtbox_InvincibilityStopped()
    {
        _animationPlayer.Play("Stop");
    }

}
