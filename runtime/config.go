package tlink

import (
	"io/ioutil"
	"path/filepath"

	"gopkg.in/yaml.v2"
)

// ParseConf read the yaml file and populate the Config instancce
func ParseConf(file string) (*TrunksConfig, error) {
	var trunksConfig *TrunksConfig
	path, err := filepath.Abs(file)
	yamlFile, err := ioutil.ReadFile(path)
	if err != nil {
		return nil, err
	}
	if err := yaml.Unmarshal(yamlFile, &trunksConfig); err != nil {
		return nil, err
	}
	return trunksConfig, nil
}

// Interfaces struct (interface name or IP address)
type NIC struct {
	ST string `yaml:"st"`
	GW string `yaml:"gw"`
}

// Bandwidth struct (kbps)
type Bandwidth struct {
	Forward float64 `yaml:"forward"`
	Return  float64 `yaml:"return"`
}

// Delay struct (ms)
type Delay struct {
	Denomination  float64 `yaml:"denomination"`
	Jitter float64 `yaml:"jitter"`
}

// ACM struct (Duration: times)
type ACM struct {
	Weight   float64 `yaml:"weight"`
	Duration int     `yaml:"duration"`
}

// TrunksConfig struct
type TrunksConfig struct {
	NIC                NIC       `yaml:"nic"`
	Bandwidth          Bandwidth `yaml:"bandwidth"`
	Delay              Delay     `yaml:"delay"`
	ACMList            []*ACM    `yaml:"acm"`
	QoS                bool
	Logs               string
	KernelVersionCheck bool
	ACMEnabled         bool
	ACMCounter         int
	ACMIndex           int
	CurrentACM         *ACM
}
