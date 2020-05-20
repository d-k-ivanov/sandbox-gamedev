using Godot;
using System;

public class HealthUI : Control
{
    [Export]
    public int MaxHealth
    {
        get;
        set;
    }

    [Export]
    public int Health
    {
        get;
        set;
    }

    private Label _label;

    public override void _Ready()
    {
        MaxHealth = 4;
        Health = 4;
        _label = GetNode<Label>("Label");
    }

    public override void _Process(float delta)
    {}
}
