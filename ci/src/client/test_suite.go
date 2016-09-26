package tests

import (
	"testing"

	"gopkg.in/check.v1"
)

func Test(t *testing.T) { TestingT(t) }

var _ = check.Suite(&ClientSuite{})

type ClientSuite struct {
}

func (cs *ClientSuite) SetUpSuite(c *check.C) {
}

func (cs *ClientSuite) TearDownSuite(c *check.C) {
}

func (cs *ClientSuite) SetUpTest(c *check.C) {
}

func (cs *ClientSuite) TearDownTest(c *check.C) {
}
