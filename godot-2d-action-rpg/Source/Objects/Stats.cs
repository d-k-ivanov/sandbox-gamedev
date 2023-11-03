using Godot;
using System;

public partial class Stats : Node
{
    // Kill v.2 - Signals
    [Signal] public delegate void NoHealthEventHandler();
    [Signal] public delegate void HealthChangedEventHandler();
    [Signal] public delegate void MaxHealthChangedEventHandler();

    private int _health;
    public int Health
    {
        get => _health;
        set
        {
            _health = value;
            EmitSignal("HealthChanged", _health);
            // EmitSignal("HealthChanged");
            if (_health <= 0)
            {
                EmitSignal("NoHealth");
            }
        }
    }

    private int _maxHealth = 1;
    [Export]
    public int MaxHealth
    {
        get => _maxHealth;
        set
        {
            _maxHealth = value;
            this._health = Mathf.Min(_health, _maxHealth);
            // EmitSignal("MaxHealthChanged");
            EmitSignal("MaxHealthChanged", _maxHealth);
        }
    }


    public override void _Ready()
    {
        Health = MaxHealth;
    }

    public override void _Process(double delta)
    {}

}
