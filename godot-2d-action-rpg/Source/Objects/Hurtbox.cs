using Godot;
using System;

public class Hurtbox : Area2D
{
    private readonly PackedScene _hitEffectScene = GD.Load("res://Source/Enemies/HitEffect.tscn") as PackedScene;
    private Timer _timer;
    private CollisionShape2D _collisionShape;

    [Signal] private delegate void InvincibilityStarted();
    [Signal] private delegate void InvincibilityStopped();
    private bool _invincible;

    public bool Invincible
    {
        get => _invincible;
        // set => _health = value;      // Default
        set
        {
            _invincible = value;
            EmitSignal(_invincible ? "InvincibilityStarted" : "InvincibilityStopped");
        }
    }

    public override void _Ready()
    {
        _timer          = GetNode<Timer>("Timer");
        _collisionShape = GetNode<CollisionShape2D>("CollisionShape2D");
    }

    public override void _Process(float delta)
    {}

    public void StartInvincibility(float duration)
    {
        Invincible = true;
        _timer.Start(duration);
    }

    private void _on_Timer_timeout()
    {
        Invincible = false;
    }

    private void _on_Hurtbox_InvincibilityStarted()
    {
        // SetDeferred("Monitorable",false);
        // _collisionShape.SetDeferred("Disabled", true);
        _collisionShape.Disabled = true;
    }

    private void _on_Hurtbox_InvincibilityStopped()
    {
        // Monitorable = true;
        _collisionShape.Disabled = false;
    }

    public void CreateHitEffect()
    {
        var hitEffectNode         = _hitEffectScene.Instance() as AnimatedSprite;
        GetTree().CurrentScene.AddChild(hitEffectNode);
        if (hitEffectNode != null) hitEffectNode.GlobalPosition = GlobalPosition;
    }
}
