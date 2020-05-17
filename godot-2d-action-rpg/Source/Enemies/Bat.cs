using Godot;
using System;

public class Bat : KinematicBody2D
{
    private Vector2 _knockBack = Vector2.Zero;
    private Stats _stats;

    public override void _Ready()
    {
        _stats = GetNode<Stats>("Stats");
        var status = this.Name + " initial status:\tHealth=<" + _stats.Health + "> MaxHealth=<" + _stats.MaxHealth + ">";
        GD.Print(status);
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
        var status = this.Name + " health:\t" + _stats.Health + " of " + _stats.MaxHealth;

        // Kill v.1
        // if (_stats.Health <= 0)
        // {
        //     QueueFree();
        //
        // }
        GD.Print(status);
        _knockBack = area.HitDirection * 120;
    }

    // Kill v.2 - Signals
    private void _on_Stats_NoHealth()
    {
        QueueFree();
    }

}

