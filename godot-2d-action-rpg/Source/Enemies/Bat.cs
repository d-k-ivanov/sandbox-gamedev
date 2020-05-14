using Godot;
using System;

public class Bat : KinematicBody2D
{
    private Vector2 _knockBack = Vector2.Zero;

    public override void _Ready() {}

    public override void _PhysicsProcess(float delta)
    {
        _knockBack = _knockBack.MoveToward(Vector2.Zero, 200 * delta);
        _knockBack = MoveAndSlide(_knockBack );
    }

    private void _on_Hurtbox_area_entered(SwordHitbox area)
    {
        _knockBack = area.HitDirection * 150;
    }
}
