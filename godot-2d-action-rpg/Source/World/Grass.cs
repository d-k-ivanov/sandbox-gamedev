using Godot;
using System;

public class Grass : Node2D
{
    private readonly PackedScene _grassEffectScene = GD.Load("res://Source/World/GrassEffect.tscn") as PackedScene;

    public override void _Ready()
    {
    }

    public override void _Process(float delta)
    {
        // if (Input.IsActionJustPressed("attack"))
        // {}
    }

    private void CreateGrassEffect()
    {
        var grassEffectNode         = _grassEffectScene.Instance() as Effect;
        GetParent().AddChild(grassEffectNode);
        if (grassEffectNode != null) grassEffectNode.GlobalPosition = GlobalPosition;
    }
    private void _on_Hurtbox_area_entered(object area)
    {
        QueueFree();
        CreateGrassEffect();
    }
}
