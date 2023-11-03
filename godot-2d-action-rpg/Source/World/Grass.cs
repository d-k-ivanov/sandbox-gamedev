using Godot;
using System;

public partial class Grass : Node2D
{
    private readonly PackedScene _grassEffectScene = GD.Load("res://Source/World/GrassEffect.tscn") as PackedScene;

    public override void _Ready()
    {
    }

    public override void _Process(double delta)
    {
        // if (Input.IsActionJustPressed("attack"))
        // {}
    }

    private void CreateGrassEffect()
    {
        var grassEffectNode         = _grassEffectScene.Instantiate() as Effect;
        GetParent().AddChild(grassEffectNode);
        if (grassEffectNode != null) grassEffectNode.GlobalPosition = GlobalPosition;
    }
    private void _on_Hurtbox_area_entered(object area)
    {
        QueueFree();
        CreateGrassEffect();
    }
}
