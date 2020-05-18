using Godot;
using System;

public class Hurtbox : Area2D
{
    [Export] public bool ShowHitEffect = true;

    private readonly PackedScene _hitEffectScene = GD.Load("res://Source/Enemies/HitEffect.tscn") as PackedScene;

    public override void _Ready()
    {}

    public override void _Process(float delta)
    {}

    private void CreateHitEffect()
    {
        var hitEffectNode         = _hitEffectScene.Instance() as AnimatedSprite;
        GetTree().CurrentScene.AddChild(hitEffectNode);
        if (hitEffectNode != null) hitEffectNode.GlobalPosition = GlobalPosition;
    }

    private void _on_Hurtbox_area_entered(object area)
    {
        if (ShowHitEffect)
        {
            CreateHitEffect();
        }
    }
}
