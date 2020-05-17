using Godot;
using System;

public class Bat : KinematicBody2D
{
    private Vector2 _knockBack = Vector2.Zero;
    private Stats _stats;

    private readonly PackedScene _enemyDeathEffectScene = GD.Load("res://Source/Enemies/EnemyDeathEffect.tscn") as PackedScene;

    public override void _Ready()
    {
        _stats = GetNode<Stats>("Stats");
        // GD.Print($"{this.Name} initial status:\tHealth={_stats.Health} MaxHealth={_stats.MaxHealth}");
    }

    public override void _PhysicsProcess(float delta)
    {
        _knockBack = _knockBack.MoveToward(Vector2.Zero, 200 * delta);
        _knockBack = MoveAndSlide(_knockBack );
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

