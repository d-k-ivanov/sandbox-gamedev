using Godot;
using System;

public class Stats : Node
{
    [Export] public int MaxHealth    = 1;

    // Kill v.2 - Signals
    [Signal] private delegate void NoHealth();

    private int _health;

    public int Health
    {
        get => _health;
        // set => _health = value;      // Default
        set
        {
            _health = value;
            if (_health <= 0)
            {
                EmitSignal("NoHealth");
            }
        }
    }

    public override void _Ready()
    {
        Health = MaxHealth;
    }

    public override void _Process(float delta)
    {}

}
